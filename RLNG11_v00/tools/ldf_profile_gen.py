#!/usr/bin/env python3
"""Generate an AmbientDevice LinLayout profile from an LDF plus JSON semantics.

The LDF supplies transport facts (baud rate, nodes, signals, frames, NADs and
schedule tables).  The JSON overlay supplies application facts that an LDF
cannot express, such as UI target masks, logical signal meanings, colors and
proprietary diagnostic DIDs.

Only the Python 3 standard library is used.  The generated C++ deliberately
uses C++11 aggregate initializers so it can be added to the existing Qt 5
project without requiring an ARM compiler at generation time.
"""

from __future__ import annotations

import argparse
import hashlib
import json
import math
import os
import re
import sys
import tempfile
from dataclasses import dataclass, field
from pathlib import Path
from typing import Any, Dict, Iterable, List, Mapping, MutableMapping, Optional, Sequence, Tuple


GENERATOR_VERSION = "1.2"
OUTPUT_HEADER = "linprofile_generated.h"
OUTPUT_SOURCE = "linprofile_generated.cpp"
OUTPUT_REPORT = "linprofile_report.json"


class ProfileError(Exception):
    """A user-facing parse, validation or generation failure."""


@dataclass(frozen=True)
class Token:
    kind: str
    value: str
    line: int
    column: int

    @property
    def where(self) -> str:
        return "line {0}, column {1}".format(self.line, self.column)


@dataclass(frozen=True)
class Signal:
    name: str
    size: int
    initial: int
    publisher: Optional[str]
    subscribers: Tuple[str, ...]
    line: int
    diagnostic: bool = False


@dataclass(frozen=True)
class FrameSignal:
    signal: str
    start_bit: int
    line: int


@dataclass(frozen=True)
class Frame:
    name: str
    frame_id: int
    publisher: Optional[str]
    length: int
    signals: Tuple[FrameSignal, ...]
    line: int
    diagnostic: bool = False


@dataclass(frozen=True)
class NodeAttribute:
    name: str
    configured_nad: Optional[int]
    initial_nad: Optional[int]
    protocol: Optional[str]
    product_id: Tuple[int, ...]
    response_error: Optional[str]
    p2_min_ms: Optional[float]
    st_min_ms: Optional[float]
    line: int


@dataclass(frozen=True)
class ScheduleEntry:
    frame: str
    delay_ms: float
    line: int


@dataclass(frozen=True)
class ScheduleTable:
    name: str
    entries: Tuple[ScheduleEntry, ...]
    line: int


@dataclass
class LdfNetwork:
    source_name: str
    source_encoding: str
    source_sha256: str
    protocol_version: str = ""
    language_version: str = ""
    baud_rate: int = 0
    master: str = ""
    slaves: List[str] = field(default_factory=list)
    signals: Dict[str, Signal] = field(default_factory=dict)
    diagnostic_signals: Dict[str, Signal] = field(default_factory=dict)
    frames: Dict[str, Frame] = field(default_factory=dict)
    diagnostic_frames: Dict[str, Frame] = field(default_factory=dict)
    node_attributes: Dict[str, NodeAttribute] = field(default_factory=dict)
    schedules: Dict[str, ScheduleTable] = field(default_factory=dict)
    warnings: List[str] = field(default_factory=list)


_TOKEN_RE = re.compile(
    r"""
    (?P<space>\s+)
  | (?P<line_comment>//[^\r\n]*)
  | (?P<block_comment>/\*.*?\*/)
  | (?P<string>"(?:\\.|[^"\\])*")
  | (?P<number>[+-]?(?:0[xX][0-9A-Fa-f]+|(?:\d+\.\d*|\.\d+|\d+)(?:[eE][+-]?\d+)?))
  | (?P<identifier>[A-Za-z_][A-Za-z0-9_]*)
  | (?P<punct>[{}:;,=()\[\]])
    """,
    re.VERBOSE | re.DOTALL,
)


def decode_ldf(path: Path) -> Tuple[str, str, bytes]:
    try:
        raw = path.read_bytes()
    except OSError as exc:
        raise ProfileError("cannot read LDF '{0}': {1}".format(path, exc))

    for encoding in ("utf-8-sig", "cp1252", "latin-1"):
        try:
            return raw.decode(encoding), encoding, raw
        except UnicodeDecodeError:
            continue
    raise ProfileError("cannot decode LDF '{0}'".format(path))


def tokenize(text: str) -> List[Token]:
    tokens: List[Token] = []
    offset = 0
    line = 1
    column = 1
    length = len(text)

    while offset < length:
        match = _TOKEN_RE.match(text, offset)
        if match is None:
            snippet = text[offset : offset + 20].replace("\n", "\\n")
            raise ProfileError(
                "unsupported LDF character at line {0}, column {1}: {2!r}".format(
                    line, column, snippet
                )
            )

        value = match.group(0)
        kind = match.lastgroup or ""
        if kind not in ("space", "line_comment", "block_comment"):
            if kind == "punct":
                kind = value
            tokens.append(Token(kind, value, line, column))

        newline_count = value.count("\n")
        if newline_count:
            line += newline_count
            column = len(value.rsplit("\n", 1)[-1]) + 1
        else:
            column += len(value)
        offset = match.end()

    tokens.append(Token("eof", "<eof>", line, column))
    return tokens


class Cursor:
    def __init__(self, tokens: Sequence[Token]):
        self.tokens = list(tokens)
        if not self.tokens or self.tokens[-1].kind != "eof":
            last = self.tokens[-1] if self.tokens else Token("eof", "<eof>", 1, 1)
            self.tokens.append(Token("eof", "<eof>", last.line, last.column))
        self.index = 0

    def peek(self, offset: int = 0) -> Token:
        index = min(self.index + offset, len(self.tokens) - 1)
        return self.tokens[index]

    def pop(self) -> Token:
        token = self.peek()
        if token.kind != "eof":
            self.index += 1
        return token

    def match(self, value: str) -> Optional[Token]:
        token = self.peek()
        if token.value == value:
            self.index += 1
            return token
        return None

    def expect(self, value: str) -> Token:
        token = self.peek()
        if token.value != value:
            raise ProfileError(
                "expected {0!r} at {1}, got {2!r}".format(value, token.where, token.value)
            )
        self.index += 1
        return token

    def expect_kind(self, kind: str, description: str) -> Token:
        token = self.peek()
        if token.kind != kind:
            raise ProfileError(
                "expected {0} at {1}, got {2!r}".format(description, token.where, token.value)
            )
        self.index += 1
        return token

    def braced_body(self) -> List[Token]:
        opening = self.expect("{")
        depth = 1
        body: List[Token] = []
        while depth:
            token = self.pop()
            if token.kind == "eof":
                raise ProfileError("unterminated block opened at {0}".format(opening.where))
            if token.value == "{":
                depth += 1
            elif token.value == "}":
                depth -= 1
                if depth == 0:
                    break
            body.append(token)
        return body

    def skip_statement(self) -> None:
        depth = 0
        while True:
            token = self.pop()
            if token.kind == "eof":
                return
            if token.value in ("{", "(", "["):
                depth += 1
            elif token.value in ("}", ")", "]"):
                if depth == 0:
                    return
                depth -= 1
            elif token.value == ";" and depth == 0:
                return


def token_int(token: Token, description: str) -> int:
    if token.kind != "number" or re.fullmatch(
        r"[+-]?(?:0[xX][0-9A-Fa-f]+|\d+)", token.value
    ) is None:
        raise ProfileError(
            "expected integer {0} at {1}, got {2!r}".format(
                description, token.where, token.value
            )
        )
    try:
        return int(token.value, 0)
    except ValueError:
        raise ProfileError("invalid integer at {0}: {1!r}".format(token.where, token.value))


def token_float(token: Token, description: str) -> float:
    if token.kind != "number":
        raise ProfileError(
            "expected numeric {0} at {1}, got {2!r}".format(
                description, token.where, token.value
            )
        )
    try:
        return float(int(token.value, 0)) if token.value.lower().startswith("0x") else float(token.value)
    except ValueError:
        raise ProfileError("invalid number at {0}: {1!r}".format(token.where, token.value))


def token_string(token: Token) -> str:
    if token.kind != "string":
        raise ProfileError("expected string at {0}, got {1!r}".format(token.where, token.value))
    try:
        return json.loads(token.value)
    except json.JSONDecodeError as exc:
        raise ProfileError("invalid string at {0}: {1}".format(token.where, exc))


def parse_nodes(tokens: Sequence[Token], network: LdfNetwork) -> None:
    cursor = Cursor(tokens)
    while cursor.peek().kind != "eof":
        key = cursor.expect_kind("identifier", "Nodes entry").value
        cursor.expect(":")
        if key == "Master":
            network.master = cursor.expect_kind("identifier", "master node name").value
            cursor.expect(",")
            token_float(cursor.pop(), "master time base")
            cursor.expect_kind("identifier", "time unit")
            cursor.expect(",")
            token_float(cursor.pop(), "master jitter")
            cursor.expect_kind("identifier", "time unit")
            cursor.expect(";")
        elif key == "Slaves":
            slaves: List[str] = []
            while True:
                slaves.append(cursor.expect_kind("identifier", "slave node name").value)
                if cursor.match(",") is None:
                    break
            cursor.expect(";")
            network.slaves = slaves
        else:
            raise ProfileError("unsupported Nodes entry {0!r}".format(key))


def parse_signals(tokens: Sequence[Token], diagnostic: bool) -> Dict[str, Signal]:
    result: Dict[str, Signal] = {}
    cursor = Cursor(tokens)
    while cursor.peek().kind != "eof":
        name_token = cursor.expect_kind("identifier", "signal name")
        cursor.expect(":")
        size = token_int(cursor.pop(), "signal size")
        cursor.expect(",")
        initial_token = cursor.peek()
        if initial_token.value == "{":
            raise ProfileError(
                "array-valued signal {0!r} at {1} is not supported by LinLayout".format(
                    name_token.value, initial_token.where
                )
            )
        initial = token_int(cursor.pop(), "signal initial value")

        publisher: Optional[str] = None
        subscribers: List[str] = []
        if diagnostic:
            cursor.expect(";")
        else:
            cursor.expect(",")
            publisher = cursor.expect_kind("identifier", "signal publisher").value
            while cursor.match(",") is not None:
                subscribers.append(cursor.expect_kind("identifier", "signal subscriber").value)
            cursor.expect(";")

        if name_token.value in result:
            raise ProfileError("duplicate signal {0!r}".format(name_token.value))
        result[name_token.value] = Signal(
            name=name_token.value,
            size=size,
            initial=initial,
            publisher=publisher,
            subscribers=tuple(subscribers),
            line=name_token.line,
            diagnostic=diagnostic,
        )
    return result


def parse_frame_mappings(tokens: Sequence[Token]) -> Tuple[FrameSignal, ...]:
    mappings: List[FrameSignal] = []
    cursor = Cursor(tokens)
    while cursor.peek().kind != "eof":
        signal_token = cursor.expect_kind("identifier", "frame signal name")
        cursor.expect(",")
        start_bit = token_int(cursor.pop(), "signal start bit")
        cursor.expect(";")
        mappings.append(FrameSignal(signal_token.value, start_bit, signal_token.line))
    return tuple(mappings)


def parse_frames(tokens: Sequence[Token], diagnostic: bool) -> Dict[str, Frame]:
    result: Dict[str, Frame] = {}
    cursor = Cursor(tokens)
    while cursor.peek().kind != "eof":
        name_token = cursor.expect_kind("identifier", "frame name")
        cursor.expect(":")
        frame_id = token_int(cursor.pop(), "raw frame ID")

        publisher: Optional[str] = None
        length = 8
        if not diagnostic:
            cursor.expect(",")
            publisher = cursor.expect_kind("identifier", "frame publisher").value
            cursor.expect(",")
            length = token_int(cursor.pop(), "frame length")

        body = cursor.braced_body()
        cursor.match(";")
        if name_token.value in result:
            raise ProfileError("duplicate frame {0!r}".format(name_token.value))
        result[name_token.value] = Frame(
            name=name_token.value,
            frame_id=frame_id,
            publisher=publisher,
            length=length,
            signals=parse_frame_mappings(body),
            line=name_token.line,
            diagnostic=diagnostic,
        )
    return result


def parse_node_attribute_block(name_token: Token, tokens: Sequence[Token]) -> NodeAttribute:
    cursor = Cursor(tokens)
    configured_nad: Optional[int] = None
    initial_nad: Optional[int] = None
    protocol: Optional[str] = None
    product_id: List[int] = []
    response_error: Optional[str] = None
    p2_min_ms: Optional[float] = None
    st_min_ms: Optional[float] = None

    while cursor.peek().kind != "eof":
        key_token = cursor.expect_kind("identifier", "node attribute")
        key = key_token.value
        if cursor.peek().value == "{":
            cursor.braced_body()
            cursor.match(";")
            continue
        cursor.expect("=")

        if key in ("configured_NAD", "initial_NAD"):
            value = token_int(cursor.pop(), key)
            cursor.expect(";")
            if key == "configured_NAD":
                configured_nad = value
            else:
                initial_nad = value
        elif key == "LIN_protocol":
            protocol = token_string(cursor.pop())
            cursor.expect(";")
        elif key == "product_id":
            while True:
                product_id.append(token_int(cursor.pop(), "product ID component"))
                if cursor.match(",") is None:
                    break
            cursor.expect(";")
        elif key == "response_error":
            response_error = cursor.expect_kind("identifier", "response-error signal").value
            cursor.expect(";")
        elif key in ("P2_min", "ST_min"):
            value = token_float(cursor.pop(), key)
            unit = cursor.expect_kind("identifier", key + " unit")
            cursor.expect(";")
            if unit.value != "ms":
                raise ProfileError(
                    "unsupported {0} unit {1!r} at {2}".format(
                        key, unit.value, unit.where
                    )
                )
            if key == "P2_min":
                p2_min_ms = value
            else:
                st_min_ms = value
        else:
            cursor.skip_statement()

    return NodeAttribute(
        name=name_token.value,
        configured_nad=configured_nad,
        initial_nad=initial_nad,
        protocol=protocol,
        product_id=tuple(product_id),
        response_error=response_error,
        p2_min_ms=p2_min_ms,
        st_min_ms=st_min_ms,
        line=name_token.line,
    )


def parse_node_attributes(tokens: Sequence[Token]) -> Dict[str, NodeAttribute]:
    result: Dict[str, NodeAttribute] = {}
    cursor = Cursor(tokens)
    while cursor.peek().kind != "eof":
        name_token = cursor.expect_kind("identifier", "node-attribute node name")
        body = cursor.braced_body()
        cursor.match(";")
        if name_token.value in result:
            raise ProfileError("duplicate Node_attributes block for {0!r}".format(name_token.value))
        result[name_token.value] = parse_node_attribute_block(name_token, body)
    return result


def parse_schedules(tokens: Sequence[Token]) -> Dict[str, ScheduleTable]:
    result: Dict[str, ScheduleTable] = {}
    cursor = Cursor(tokens)
    while cursor.peek().kind != "eof":
        table_token = cursor.expect_kind("identifier", "schedule table name")
        table_body = Cursor(cursor.braced_body())
        cursor.match(";")
        entries: List[ScheduleEntry] = []
        while table_body.peek().kind != "eof":
            frame_token = table_body.expect_kind("identifier", "scheduled frame name")
            if table_body.peek().value != "delay":
                raise ProfileError(
                    "unsupported schedule command {0!r} at {1}; only '<frame> delay <ms> ms;' is supported".format(
                        frame_token.value, frame_token.where
                    )
                )
            table_body.expect("delay")
            delay_ms = token_float(table_body.pop(), "schedule delay")
            unit = table_body.expect_kind("identifier", "schedule delay unit")
            if unit.value != "ms":
                raise ProfileError(
                    "unsupported schedule delay unit {0!r} at {1}".format(unit.value, unit.where)
                )
            table_body.expect(";")
            entries.append(ScheduleEntry(frame_token.value, delay_ms, frame_token.line))

        if table_token.value in result:
            raise ProfileError("duplicate schedule table {0!r}".format(table_token.value))
        result[table_token.value] = ScheduleTable(
            table_token.value, tuple(entries), table_token.line
        )
    return result


def parse_ldf(path: Path) -> LdfNetwork:
    text, encoding, raw = decode_ldf(path)
    network = LdfNetwork(
        source_name=path.name,
        source_encoding=encoding,
        source_sha256=hashlib.sha256(raw).hexdigest(),
    )
    cursor = Cursor(tokenize(text))

    section_parsers = {
        "Nodes": lambda body: parse_nodes(body, network),
        "Signals": lambda body: setattr(network, "signals", parse_signals(body, False)),
        "Diagnostic_signals": lambda body: setattr(
            network, "diagnostic_signals", parse_signals(body, True)
        ),
        "Frames": lambda body: setattr(network, "frames", parse_frames(body, False)),
        "Diagnostic_frames": lambda body: setattr(
            network, "diagnostic_frames", parse_frames(body, True)
        ),
        "Node_attributes": lambda body: setattr(
            network, "node_attributes", parse_node_attributes(body)
        ),
        "Schedule_tables": lambda body: setattr(
            network, "schedules", parse_schedules(body)
        ),
    }

    while cursor.peek().kind != "eof":
        name_token = cursor.expect_kind("identifier", "top-level LDF item")
        name = name_token.value

        if name == "LIN_description_file":
            cursor.expect(";")
        elif name in ("LIN_protocol_version", "LIN_language_version"):
            cursor.expect("=")
            value = token_string(cursor.pop())
            cursor.expect(";")
            if name == "LIN_protocol_version":
                network.protocol_version = value
            else:
                network.language_version = value
        elif name == "LIN_speed":
            cursor.expect("=")
            speed = token_float(cursor.pop(), "LIN speed")
            unit = cursor.expect_kind("identifier", "LIN speed unit")
            cursor.expect(";")
            if unit.value == "kbps":
                speed *= 1000.0
            elif unit.value != "bps":
                raise ProfileError(
                    "unsupported LIN speed unit {0!r} at {1}".format(unit.value, unit.where)
                )
            rounded = int(round(speed))
            if not math.isclose(speed, rounded, rel_tol=0.0, abs_tol=1e-6):
                raise ProfileError("LIN speed does not resolve to an integer baud rate")
            network.baud_rate = rounded
        elif cursor.peek().value == "{":
            body = cursor.braced_body()
            cursor.match(";")
            parser = section_parsers.get(name)
            if parser is not None:
                parser(body)
            elif name in ("Sporadic_frames", "Event_triggered_frames"):
                network.warnings.append(
                    "LDF contains {0}; AmbientDevice schedule generation does not support it".format(
                        name
                    )
                )
            # Encoding/representation and other sections are not needed for raw layout.
        else:
            cursor.skip_statement()

    validate_ldf(network)
    return network


def validate_ldf(network: LdfNetwork) -> None:
    errors: List[str] = []
    known_nodes = set(network.slaves)
    if network.master:
        known_nodes.add(network.master)

    if not network.protocol_version:
        errors.append("LIN_protocol_version is missing")
    if network.baud_rate <= 0:
        errors.append("LIN_speed is missing or zero")
    if not network.master:
        errors.append("Nodes.Master is missing")
    if not network.slaves:
        errors.append("Nodes.Slaves is empty")

    for signal in network.signals.values():
        if signal.size <= 0 or signal.size > 64:
            errors.append("signal {0}: size must be 1..64 bits".format(signal.name))
        maximum = (1 << signal.size) - 1 if 0 < signal.size <= 64 else 0
        if signal.initial < 0 or signal.initial > maximum:
            errors.append("signal {0}: initial value does not fit".format(signal.name))
        if signal.publisher not in known_nodes:
            errors.append("signal {0}: unknown publisher {1!r}".format(signal.name, signal.publisher))
        for subscriber in signal.subscribers:
            if subscriber not in known_nodes:
                errors.append(
                    "signal {0}: unknown subscriber {1!r}".format(signal.name, subscriber)
                )

    for signal in network.diagnostic_signals.values():
        if signal.size <= 0 or signal.size > 64:
            errors.append("diagnostic signal {0}: size must be 1..64 bits".format(signal.name))
        maximum = (1 << signal.size) - 1 if 0 < signal.size <= 64 else 0
        if signal.initial < 0 or signal.initial > maximum:
            errors.append("diagnostic signal {0}: initial value does not fit".format(signal.name))

    def validate_frames(frames: Mapping[str, Frame], signals: Mapping[str, Signal]) -> None:
        for frame in frames.values():
            if frame.frame_id < 0 or frame.frame_id > 0x3F:
                errors.append(
                    "frame {0}: raw ID {1:#x} is outside 0x00..0x3F (do not supply a protected PID)".format(
                        frame.name, frame.frame_id
                    )
                )
            if frame.length <= 0 or frame.length > 8:
                errors.append("frame {0}: length must be 1..8".format(frame.name))
            if not frame.diagnostic and frame.publisher not in known_nodes:
                errors.append(
                    "frame {0}: unknown publisher {1!r}".format(frame.name, frame.publisher)
                )

            used_bits: Dict[int, str] = {}
            used_signals: set[str] = set()
            for mapping in frame.signals:
                signal = signals.get(mapping.signal)
                if signal is None:
                    errors.append(
                        "frame {0}: references unknown signal {1}".format(
                            frame.name, mapping.signal
                        )
                    )
                    continue
                if mapping.signal in used_signals:
                    errors.append(
                        "frame {0}: signal {1} appears more than once".format(
                            frame.name, mapping.signal
                        )
                    )
                used_signals.add(mapping.signal)
                if mapping.start_bit < 0 or mapping.start_bit + signal.size > frame.length * 8:
                    errors.append(
                        "frame {0}: signal {1} [{2}..{3}] exceeds {4}-byte frame".format(
                            frame.name,
                            signal.name,
                            mapping.start_bit,
                            mapping.start_bit + signal.size - 1,
                            frame.length,
                        )
                    )
                    continue
                if not frame.diagnostic and signal.publisher != frame.publisher:
                    errors.append(
                        "frame {0}: signal {1} publisher {2!r} differs from frame publisher {3!r}".format(
                            frame.name, signal.name, signal.publisher, frame.publisher
                        )
                    )
                for bit in range(mapping.start_bit, mapping.start_bit + signal.size):
                    previous = used_bits.get(bit)
                    if previous is not None:
                        errors.append(
                            "frame {0}: signals {1} and {2} overlap at bit {3}".format(
                                frame.name, previous, signal.name, bit
                            )
                        )
                        break
                    used_bits[bit] = signal.name

    validate_frames(network.frames, network.signals)
    validate_frames(network.diagnostic_frames, network.diagnostic_signals)

    all_frame_names = set(network.frames) | set(network.diagnostic_frames)
    for table in network.schedules.values():
        if not table.entries:
            errors.append("schedule table {0} is empty".format(table.name))
        for entry in table.entries:
            if entry.frame not in all_frame_names:
                errors.append(
                    "schedule {0}: references unknown frame {1} at line {2}".format(
                        table.name, entry.frame, entry.line
                    )
                )
            if entry.delay_ms < 0 or entry.delay_ms > 1000:
                errors.append(
                    "schedule {0}: delay for {1} must be 0..1000 ms for LinLayout".format(
                        table.name, entry.frame
                    )
                )

    for node_name, attributes in network.node_attributes.items():
        if node_name not in network.slaves:
            errors.append("Node_attributes contains unknown slave {0}".format(node_name))
        for label, value in (
            ("configured_NAD", attributes.configured_nad),
            ("initial_NAD", attributes.initial_nad),
        ):
            if value is not None and not 0 <= value <= 0xFF:
                errors.append("node {0}: {1} is outside one byte".format(node_name, label))

    id_users: Dict[int, List[str]] = {}
    for frame in list(network.frames.values()) + list(network.diagnostic_frames.values()):
        id_users.setdefault(frame.frame_id, []).append(frame.name)
    for frame_id, names in sorted(id_users.items()):
        if len(names) > 1:
            network.warnings.append(
                "raw ID 0x{0:02X} is shared by frames: {1}; generation will reject a selected schedule that uses conflicting frames".format(
                    frame_id, ", ".join(names)
                )
            )

    if errors:
        raise ProfileError("invalid LDF:\n  - " + "\n  - ".join(errors))


def read_json(path: Path) -> Dict[str, Any]:
    try:
        with path.open("r", encoding="utf-8") as stream:
            value = json.load(stream)
    except OSError as exc:
        raise ProfileError("cannot read overlay '{0}': {1}".format(path, exc))
    except json.JSONDecodeError as exc:
        raise ProfileError("invalid JSON overlay '{0}': {1}".format(path, exc))
    if not isinstance(value, dict):
        raise ProfileError("overlay root must be a JSON object: {0}".format(path))
    return value


def deep_merge(target: MutableMapping[str, Any], source: Mapping[str, Any]) -> None:
    for key, value in source.items():
        if isinstance(value, dict) and isinstance(target.get(key), dict):
            deep_merge(target[key], value)
        else:
            target[key] = value


def load_overlays(paths: Sequence[Path]) -> Tuple[Dict[str, Any], str]:
    if not paths:
        raise ProfileError("at least one --overlay is required")
    merged: Dict[str, Any] = {}
    for path in paths:
        deep_merge(merged, read_json(path))
    canonical = json.dumps(merged, ensure_ascii=False, sort_keys=True, separators=(",", ":"))
    return merged, hashlib.sha256(canonical.encode("utf-8")).hexdigest()


def config_object(value: Any, path: str) -> Dict[str, Any]:
    if not isinstance(value, dict):
        raise ProfileError("{0} must be a JSON object".format(path))
    return value


def config_list(value: Any, path: str) -> List[Any]:
    if not isinstance(value, list):
        raise ProfileError("{0} must be a JSON array".format(path))
    return value


def config_string(value: Any, path: str) -> str:
    if not isinstance(value, str) or not value or value.startswith("TODO"):
        raise ProfileError("{0} must be a non-empty resolved string".format(path))
    return value


def config_bool(value: Any, path: str) -> bool:
    if not isinstance(value, bool):
        raise ProfileError("{0} must be true or false".format(path))
    return value


def config_int(value: Any, path: str, minimum: int, maximum: int) -> int:
    if isinstance(value, bool):
        raise ProfileError("{0} must be an integer".format(path))
    if isinstance(value, str):
        if value.startswith("TODO"):
            raise ProfileError("{0} is unresolved".format(path))
        try:
            result = int(value, 0)
        except ValueError:
            raise ProfileError("{0} is not an integer: {1!r}".format(path, value))
    elif isinstance(value, int):
        result = value
    else:
        raise ProfileError("{0} must be an integer or 0x-prefixed string".format(path))
    if result < minimum or result > maximum:
        raise ProfileError(
            "{0} must be in range {1}..{2}, got {3}".format(path, minimum, maximum, result)
        )
    return result


def optional_int(
    obj: Mapping[str, Any], key: str, path: str, minimum: int, maximum: int, default: int
) -> int:
    return config_int(obj.get(key, default), path + "." + key, minimum, maximum)


def require_ascii(value: str, path: str) -> str:
    try:
        value.encode("ascii")
    except UnicodeEncodeError:
        raise ProfileError(
            "{0} must currently be ASCII because LinLayout stores const char*: {1!r}".format(
                path, value
            )
        )
    return value


LOGICAL_SIGNAL_ENUMS = {
    "target_mask": "ELinSignalTargetMask",
    "direct_rgb_enable": "ELinSignalDirectRgbEnable",
    "led_enable": "ELinSignalLedEnable",
    "fading_enable": "ELinSignalFadingEnable",
    "fading_time": "ELinSignalFadingTime",
    "intensity": "ELinSignalIntensity",
    "red_or_predefined": "ELinSignalRedOrPredefinedColor",
    "green": "ELinSignalGreen",
    "blue": "ELinSignalBlue",
    "group_addressing": "ELinSignalGroupAddressing",
    "intensity_normalization": "ELinSignalIntensityNormalization",
    "dimming_time_relative": "ELinSignalDimmingTimeRelative",
    "dimming_time_base_250ms": "ELinSignalDimmingTimeBase250ms",
    "special_function": "ELinSignalSpecialFunction",
    "command_validity": "ELinSignalCommandValidity",
    "raw": "ELinSignalRawValue",
}

STATUS_FIELD_ENUMS = {
    "red_output_error": "ELinStatusRedOutputError",
    "green_output_error": "ELinStatusGreenOutputError",
    "blue_output_error": "ELinStatusBlueOutputError",
    "temperature_error": "ELinStatusTemperatureError",
    "voltage_error": "ELinStatusVoltageError",
    "communication_error": "ELinStatusCommunicationError",
    "raw_value": "ELinStatusRawValue",
}

NODE_TYPE_ENUMS = {"rgb": "ENodeTypeRGB", "white": "ENodeTypeWhite"}

OPERATION_ENUMS = {
    "lock": "EOperationTypeLock",
    "single_address": "EOperationTypeSingleAddr",
    "group_address": "EOperationTypeGroupAddr",
    "platform": "EOperationTypePlatform",
    "intensity": "EOperationTypeIntensity",
    "red_calibration": "EOperationTypeRValue",
    "green_calibration": "EOperationTypeGValue",
    "blue_calibration": "EOperationTypeBValue",
    "part_number": "EOperationTypePartNO",
    "serial_number": "EOperationTypeSerialNO",
    "calibration_mode": "EOperationTypeCalibration",
    "software_version": "EOperationTypeSoftwareVer",
    "hardware_version": "EOperationTypeHardwareVer",
    "product_id": "EOperationTypeID",
    "raw": "EOperationTypeRaw",
}

PROTOCOL_ENUMS = {
    "read_by_identifier": "ELinDiagnosticReadByIdentifier",
    "product_identification": "ELinDiagnosticProductIdentification",
}

CHECKSUM_ENUMS = {"classic": "ELinChecksumClassic", "enhanced": "ELinChecksumEnhanced"}

COLOR_MODEL_ENUMS = {
    "predefined_only": "ELinColorModelPredefinedOnly",
    "direct_rgb_only": "ELinColorModelDirectRgbOnly",
    "selectable": "ELinColorModelSelectable",
}

ADDRESSING_MODEL_ENUMS = {
    "fixed_target_mask": "ELinAddressingModelFixedTargetMask",
    "address_value_and_group_bit": "ELinAddressingModelAddressValueAndGroupBit",
}

DIMMING_MODEL_ENUMS = {
    "none": "ELinDimmingModelNone",
    "fade_enable_and_time": "ELinDimmingModelFadeEnableAndTime",
    "ramp_with_time_base": "ELinDimmingModelRampWithTimeBase",
    "ramp_time_only": "ELinDimmingModelRampTimeOnly",
}

DIAGNOSTIC_MODEL_ENUMS = {
    "none": "ELinDiagnosticModelNone",
    "lin20_node_configuration": "ELinDiagnosticModelLin20NodeConfiguration",
    "custom_did": "ELinDiagnosticModelCustomDid",
}

BOOLEAN_LOGICAL_SIGNALS = {
    "direct_rgb_enable",
    "led_enable",
    "fading_enable",
    "group_addressing",
    "intensity_normalization",
    "dimming_time_relative",
    "dimming_time_base_250ms",
    "command_validity",
}

LOGICAL_STORAGE_BITS = {
    key: (
        32 if key == "raw"
        else 16 if key == "target_mask"
        else 1 if key in BOOLEAN_LOGICAL_SIGNALS
        else 8
    )
    for key in LOGICAL_SIGNAL_ENUMS
}


def checksum_name(network: LdfNetwork, frame_name: str, overlay: Mapping[str, Any]) -> str:
    overrides = overlay.get("checksum_overrides", {})
    if not isinstance(overrides, dict):
        raise ProfileError("checksum_overrides must be an object")
    value = overrides.get(frame_name)
    if value is None:
        try:
            major = int(network.protocol_version.split(".", 1)[0])
        except (ValueError, IndexError):
            raise ProfileError("unsupported LIN protocol version {0!r}".format(network.protocol_version))
        value = "enhanced" if major >= 2 else "classic"
    if value not in CHECKSUM_ENUMS:
        raise ProfileError(
            "checksum for frame {0} must be 'classic' or 'enhanced'".format(frame_name)
        )
    return value


def write_raw_bits(payload: List[int], start_bit: int, bit_length: int, value: int) -> None:
    for bit in range(bit_length):
        byte_index = (start_bit + bit) // 8
        mask = 1 << ((start_bit + bit) % 8)
        if value & (1 << bit):
            payload[byte_index] |= mask
        else:
            payload[byte_index] &= ~mask


def frame_mapping(network: LdfNetwork, frame: Frame, signal_name: str) -> Tuple[Signal, FrameSignal]:
    signal = network.signals.get(signal_name)
    if signal is None:
        raise ProfileError("control/status mapping references unknown signal {0!r}".format(signal_name))
    matches = [item for item in frame.signals if item.signal == signal_name]
    if len(matches) != 1:
        raise ProfileError(
            "signal {0!r} is not mapped exactly once in frame {1!r}".format(
                signal_name, frame.name
            )
        )
    return signal, matches[0]


def find_signal_frame(network: LdfNetwork, signal_name: str, candidate_frames: Sequence[str]) -> str:
    matches = [
        name
        for name in candidate_frames
        if any(mapping.signal == signal_name for mapping in network.frames[name].signals)
    ]
    if len(matches) != 1:
        raise ProfileError(
            "signal {0!r} must occur in exactly one selected master frame; found {1}".format(
                signal_name, matches
            )
        )
    return matches[0]


def parse_palette(value: Any) -> List[Tuple[int, int, int]]:
    entries = config_list(value, "color.palette")
    result: List[Tuple[int, int, int]] = []
    for index, entry in enumerate(entries):
        path = "color.palette[{0}]".format(index)
        if isinstance(entry, str):
            if not re.fullmatch(r"[0-9A-Fa-f]{6}", entry):
                raise ProfileError("{0} must be six hexadecimal RGB digits".format(path))
            result.append((int(entry[0:2], 16), int(entry[2:4], 16), int(entry[4:6], 16)))
        elif isinstance(entry, list) and len(entry) == 3:
            result.append(
                tuple(config_int(channel, path, 0, 255) for channel in entry)  # type: ignore[arg-type]
            )
        else:
            raise ProfileError("{0} must be 'RRGGBB' or [R,G,B]".format(path))
    if len(result) > 30:
        raise ProfileError(
            "color.palette may contain at most 30 colors because the current UI exposes 30 buttons"
        )
    return result


def parse_status_rule(value: Any, path: str) -> Dict[str, Any]:
    """Normalize one application status rule without inventing multibit encodings."""
    if isinstance(value, str):
        semantic = value
        normal_value: Optional[int] = None
        error_value: Optional[int] = None
        any_non_normal_is_error = False
    else:
        rule = config_object(value, path)
        semantic = config_string(rule.get("field"), path + ".field")
        if "normal_value" not in rule or "error_value" not in rule:
            raise ProfileError(
                "{0} requires explicit normal_value and error_value".format(path)
            )
        normal_value = config_int(
            rule["normal_value"], path + ".normal_value", 0, 0xFFFFFFFF
        )
        error_value = config_int(
            rule["error_value"], path + ".error_value", 0, 0xFFFFFFFF
        )
        any_non_normal_is_error = config_bool(
            rule.get("any_non_normal_is_error", False),
            path + ".any_non_normal_is_error",
        )
    if semantic not in STATUS_FIELD_ENUMS:
        raise ProfileError(
            "{0}.field must be one of: {1}".format(
                path, ", ".join(STATUS_FIELD_ENUMS)
            )
        )
    return {
        "semantic": semantic,
        "normal_value": normal_value,
        "error_value": error_value,
        "any_non_normal_is_error": any_non_normal_is_error,
    }


def build_profile(network: LdfNetwork, overlay: Dict[str, Any], overlay_sha256: str) -> Dict[str, Any]:
    if overlay.get("schema_version") != 1:
        raise ProfileError("overlay schema_version must be 1")

    profile_cfg = config_object(overlay.get("profile"), "profile")
    layout_name = require_ascii(
        config_string(profile_cfg.get("layout_name"), "profile.layout_name"),
        "profile.layout_name",
    )
    device_name = require_ascii(
        config_string(profile_cfg.get("device_name"), "profile.device_name"),
        "profile.device_name",
    )
    serial_timeout = config_int(
        profile_cfg.get("serial_io_timeout_ms"),
        "profile.serial_io_timeout_ms", 1, 1000,
    )
    diagnostic_request_delay = config_int(
        profile_cfg.get("diagnostic_request_to_response_delay_ms"),
        "profile.diagnostic_request_to_response_delay_ms", 0, 1000,
    )
    diag_gap = config_int(
        profile_cfg.get("diagnostic_inter_frame_delay_ms"),
        "profile.diagnostic_inter_frame_delay_ms", 0, 1000,
    )
    response_delay = config_int(
        profile_cfg.get("response_delay_ms"),
        "profile.response_delay_ms", 0, 1000,
    )
    post_write = config_int(
        profile_cfg.get("post_write_settle_ms"),
        "profile.post_write_settle_ms", 0, 1000,
    )
    bulk_readback_delay = config_int(
        profile_cfg.get("bulk_write_readback_delay_ms", 1000),
        "profile.bulk_write_readback_delay_ms", 0, 10000,
    )
    max_queue = config_int(
        profile_cfg.get("maximum_diagnostic_queue_depth"),
        "profile.maximum_diagnostic_queue_depth", 1, 10000,
    )
    intensity_maximum = config_int(
        profile_cfg.get("intensity_maximum"),
        "profile.intensity_maximum", 1, 0xFFFFFFFF,
    )
    dimming_maximum = config_int(
        profile_cfg.get("dimming_maximum"),
        "profile.dimming_maximum", 1, 0xFFFFFFFF,
    )

    models_cfg = config_object(overlay.get("models"), "models")
    color_model = config_string(models_cfg.get("color"), "models.color")
    addressing_model = config_string(models_cfg.get("addressing"), "models.addressing")
    dimming_model = config_string(models_cfg.get("dimming"), "models.dimming")
    diagnostic_model = config_string(models_cfg.get("diagnostic"), "models.diagnostic")
    for value, enums, path in (
        (color_model, COLOR_MODEL_ENUMS, "models.color"),
        (addressing_model, ADDRESSING_MODEL_ENUMS, "models.addressing"),
        (dimming_model, DIMMING_MODEL_ENUMS, "models.dimming"),
        (diagnostic_model, DIAGNOSTIC_MODEL_ENUMS, "models.diagnostic"),
    ):
        if value not in enums:
            raise ProfileError(
                "{0} must be one of: {1}".format(path, ", ".join(enums))
            )

    color_cfg = config_object(overlay.get("color"), "color")
    palette = parse_palette(color_cfg.get("palette", []))
    if color_model in ("predefined_only", "selectable") and not palette:
        raise ProfileError("the selected color model requires a non-empty color.palette")
    if color_model == "direct_rgb_only" and palette:
        raise ProfileError(
            "direct_rgb_only must use an empty color.palette; remove unrelated palette data"
        )

    schedule_cfg = config_object(overlay.get("schedule"), "schedule")
    schedule_name = config_string(schedule_cfg.get("table"), "schedule.table")
    primary_control_frame_name = config_string(
        schedule_cfg.get("control_frame"), "schedule.control_frame"
    )
    if schedule_name not in network.schedules:
        raise ProfileError(
            "schedule.table {0!r} does not exist; available: {1}".format(
                schedule_name, ", ".join(network.schedules)
            )
        )
    delay_mode = schedule_cfg.get("delay_mode")
    if delay_mode not in ("copy_as_post_delay", "fixed_post_delay"):
        raise ProfileError(
            "schedule.delay_mode must explicitly be 'copy_as_post_delay' or 'fixed_post_delay'"
        )
    fixed_delay = None
    if delay_mode == "fixed_post_delay":
        fixed_delay = config_int(schedule_cfg.get("post_delay_ms"), "schedule.post_delay_ms", 0, 1000)
    master_policy = schedule_cfg.get("master_frame_policy", "preserve")
    if master_policy not in ("preserve", "once_per_cycle"):
        raise ProfileError("schedule.master_frame_policy must be 'preserve' or 'once_per_cycle'")
    ignored_frames = set(config_list(schedule_cfg.get("ignored_frames", []), "schedule.ignored_frames"))
    if not all(isinstance(item, str) for item in ignored_frames):
        raise ProfileError("schedule.ignored_frames must contain frame-name strings")

    node_cfgs = config_list(overlay.get("nodes"), "nodes")
    if not node_cfgs:
        raise ProfileError("nodes must explicitly select at least one application node")

    exact_status_raw = config_object(overlay.get("status_signal_map", {}), "status_signal_map")
    suffix_status_raw = config_object(overlay.get("status_suffix_map", {}), "status_suffix_map")
    exact_status_map: Dict[str, Dict[str, Any]] = {}
    suffix_status_map: Dict[str, Dict[str, Any]] = {}
    for mapping_name, raw_mapping, normalized in (
        ("status_signal_map", exact_status_raw, exact_status_map),
        ("status_suffix_map", suffix_status_raw, suffix_status_map),
    ):
        for key, value in raw_mapping.items():
            if not isinstance(key, str) or not key:
                raise ProfileError("{0} keys must be non-empty strings".format(mapping_name))
            normalized[key] = parse_status_rule(
                value, "{0}.{1}".format(mapping_name, key)
            )
    allow_empty_status = config_bool(
        overlay.get("allow_empty_status", False), "allow_empty_status"
    )

    status_layouts: List[Dict[str, Any]] = []
    status_signatures: Dict[Tuple[Tuple[Any, ...], ...], int] = {}
    nodes: List[Dict[str, Any]] = []
    node_by_status_frame: Dict[str, int] = {}

    for index, raw_node_cfg in enumerate(node_cfgs):
        path = "nodes[{0}]".format(index)
        node_cfg = config_object(raw_node_cfg, path)
        ldf_node = config_string(node_cfg.get("ldf_node"), path + ".ldf_node")
        if ldf_node not in network.slaves:
            raise ProfileError("{0}.ldf_node is not an LDF slave: {1}".format(path, ldf_node))
        name = require_ascii(
            config_string(node_cfg.get("name", ldf_node), path + ".name"), path + ".name"
        )
        status_frame_name = config_string(node_cfg.get("status_frame"), path + ".status_frame")
        status_frame = network.frames.get(status_frame_name)
        if status_frame is None:
            raise ProfileError("{0}.status_frame is unknown: {1}".format(path, status_frame_name))
        if status_frame.publisher != ldf_node:
            raise ProfileError(
                "{0}.status_frame publisher is {1}, expected {2}".format(
                    path, status_frame.publisher, ldf_node
                )
            )
        if status_frame_name in node_by_status_frame:
            raise ProfileError("status frame {0} is assigned to more than one node".format(status_frame_name))

        attributes = network.node_attributes.get(ldf_node)
        ldf_nad = None if attributes is None else (
            attributes.initial_nad
            if attributes.initial_nad is not None
            else attributes.configured_nad
        )
        if "nad" in node_cfg:
            nad = config_int(node_cfg["nad"], path + ".nad", 1, 0x7D)
            nad_source = "overlay"
        elif ldf_nad is not None and 1 <= ldf_nad <= 0x7D:
            nad = ldf_nad
            nad_source = "LDF Node_attributes"
        else:
            raise ProfileError(
                "{0}.nad is required because LDF NAD is absent or not an operational NAD".format(path)
            )
        control_mask = config_int(node_cfg.get("control_mask"), path + ".control_mask", 1, 0xFFFF)
        if (addressing_model == "fixed_target_mask") and (control_mask & (control_mask - 1)):
            raise ProfileError("{0}.control_mask must contain exactly one bit".format(path))
        ui_slot = config_int(node_cfg.get("ui_slot"), path + ".ui_slot", 0, len(node_cfgs) - 1)
        node_type = config_string(node_cfg.get("node_type"), path + ".node_type")
        if node_type not in NODE_TYPE_ENUMS:
            raise ProfileError("{0}.node_type must be 'rgb' or 'white'".format(path))

        fields: List[Dict[str, Any]] = []
        for frame_signal in status_frame.signals:
            matches: List[Dict[str, Any]] = []
            if frame_signal.signal in exact_status_map:
                matches.append(exact_status_map[frame_signal.signal])
            for suffix, rule in suffix_status_map.items():
                if frame_signal.signal.endswith(suffix):
                    matches.append(rule)
            unique_matches: List[Dict[str, Any]] = []
            for match in matches:
                if match not in unique_matches:
                    unique_matches.append(match)
            matches = unique_matches
            if len(matches) > 1:
                raise ProfileError(
                    "status signal {0} matches conflicting semantic rules: {1}".format(
                        frame_signal.signal,
                        [match["semantic"] for match in matches],
                    )
                )
            signal = network.signals[frame_signal.signal]
            if not matches:
                fields.append(
                    {
                        "semantic": "raw_value",
                        "enum": STATUS_FIELD_ENUMS["raw_value"],
                        "name": signal.name,
                        "start_bit": frame_signal.start_bit,
                        "bit_length": signal.size,
                        "normal_value": 0,
                        "error_value": (1 << signal.size) - 1,
                        "any_non_normal_is_error": False,
                    }
                )
                continue
            rule = matches[0]
            semantic = rule["semantic"]
            normal_value = rule["normal_value"]
            error_value = rule["error_value"]
            if normal_value is None or error_value is None:
                if signal.size != 1:
                    raise ProfileError(
                        "status signal {0} is {1} bits; its rule must explicitly set normal_value and error_value".format(
                            signal.name, signal.size
                        )
                    )
                normal_value, error_value = 0, 1
            maximum = (1 << signal.size) - 1
            if normal_value > maximum or error_value > maximum:
                raise ProfileError(
                    "status values for {0} do not fit its {1}-bit width".format(
                        signal.name, signal.size
                    )
                )
            if normal_value == error_value:
                raise ProfileError(
                    "status values for {0} must differ".format(signal.name)
                )
            fields.append(
                {
                    "semantic": semantic,
                    "enum": STATUS_FIELD_ENUMS[semantic],
                    "name": signal.name,
                    "start_bit": frame_signal.start_bit,
                    "bit_length": signal.size,
                    "normal_value": normal_value,
                    "error_value": error_value,
                    "any_non_normal_is_error": rule["any_non_normal_is_error"],
                }
            )

        fields.sort(key=lambda item: (item["start_bit"], item["semantic"]))
        if not fields and not allow_empty_status:
            raise ProfileError(
                "status frame {0} has no mapped fields; add status mappings or explicitly set allow_empty_status=true".format(
                    status_frame.name
                )
            )
        signature = tuple(
            (
                item["enum"], item["start_bit"], item["bit_length"],
                item["normal_value"], item["error_value"],
                item["any_non_normal_is_error"],
            )
            for item in fields
        )
        if signature not in status_signatures:
            status_signatures[signature] = len(status_layouts)
            status_layouts.append(
                {
                    "name": "StatusLayout{0}".format(len(status_layouts)),
                    "fields": fields,
                }
            )
        status_index = status_signatures[signature]

        nodes.append(
            {
                "name": name,
                "ldf_node": ldf_node,
                "ui_slot": ui_slot,
                "nad": nad,
                "nad_source": nad_source,
                "ldf_nad": ldf_nad,
                "control_mask": control_mask,
                "status_frame": status_frame_name,
                "status_frame_id": status_frame.frame_id,
                "status_length": status_frame.length,
                "status_layout_index": status_index,
                "checksum": checksum_name(network, status_frame_name, overlay),
                "node_type": node_type,
            }
        )
        node_by_status_frame[status_frame_name] = index

    def unique_values(items: Iterable[Any], label: str) -> None:
        values = list(items)
        if len(values) != len(set(values)):
            raise ProfileError("node {0} values must be unique".format(label))

    unique_values((node["name"] for node in nodes), "name")
    unique_values((node["ui_slot"] for node in nodes), "ui_slot")
    unique_values((node["nad"] for node in nodes), "NAD")
    unique_values((node["control_mask"] for node in nodes), "control mask")
    unique_values((node["status_frame_id"] for node in nodes), "status raw frame ID")

    selected_attributes = [
        network.node_attributes.get(node["ldf_node"]) for node in nodes
    ]
    ldf_p2_values = [
        attributes.p2_min_ms
        for attributes in selected_attributes
        if attributes is not None and attributes.p2_min_ms is not None
    ]
    ldf_st_values = [
        attributes.st_min_ms
        for attributes in selected_attributes
        if attributes is not None and attributes.st_min_ms is not None
    ]
    ldf_p2_minimum = max(ldf_p2_values) if ldf_p2_values else None
    ldf_st_minimum = max(ldf_st_values) if ldf_st_values else None
    if ldf_p2_minimum is not None and diagnostic_request_delay < math.ceil(ldf_p2_minimum):
        raise ProfileError(
            "profile.diagnostic_request_to_response_delay_ms is below selected-node LDF P2_min ({0} ms)".format(
                ldf_p2_minimum
            )
        )
    if ldf_st_minimum is not None and diag_gap < math.ceil(ldf_st_minimum):
        raise ProfileError(
            "profile.diagnostic_inter_frame_delay_ms is below selected-node LDF ST_min ({0} ms)".format(
                ldf_st_minimum
            )
        )

    selected_table = network.schedules[schedule_name]
    schedule_items: List[Dict[str, Any]] = []
    published_names: List[str] = []
    seen_master_frames: set[str] = set()
    scheduled_node_indices: set[int] = set()
    skipped: List[Dict[str, Any]] = []
    for entry in selected_table.entries:
        if entry.frame in ignored_frames:
            skipped.append({"frame": entry.frame, "reason": "explicitly ignored"})
            continue
        if entry.frame in network.diagnostic_frames:
            raise ProfileError(
                "selected normal schedule {0} contains diagnostic frame {1}".format(
                    schedule_name, entry.frame
                )
            )
        frame = network.frames.get(entry.frame)
        if frame is None:
            raise ProfileError("selected schedule references unknown frame {0}".format(entry.frame))
        delay = fixed_delay if fixed_delay is not None else entry.delay_ms
        rounded_delay = int(round(delay))
        if not math.isclose(delay, rounded_delay, rel_tol=0.0, abs_tol=1e-6):
            raise ProfileError(
                "LinScheduleSlot requires integer milliseconds; {0}/{1} has {2}".format(
                    schedule_name, entry.frame, delay
                )
            )

        if frame.publisher == network.master:
            if master_policy == "once_per_cycle" and frame.name in seen_master_frames:
                skipped.append({"frame": frame.name, "reason": "master_frame_policy=once_per_cycle"})
                continue
            seen_master_frames.add(frame.name)
            if frame.name not in published_names:
                published_names.append(frame.name)
            schedule_items.append(
                {"type": "transmit", "frame": frame.name, "delay_after_ms": rounded_delay}
            )
        else:
            if frame.name not in node_by_status_frame:
                raise ProfileError(
                    "scheduled slave frame {0} has no explicit nodes[] entry; add the node or list the frame in schedule.ignored_frames".format(
                        frame.name
                    )
                )
            node_index = node_by_status_frame[frame.name]
            scheduled_node_indices.add(node_index)
            schedule_items.append(
                {"type": "poll", "node_index": node_index, "frame": frame.name, "delay_after_ms": rounded_delay}
            )

    if not schedule_items:
        raise ProfileError("generated schedule is empty")
    missing_nodes = [nodes[index]["name"] for index in range(len(nodes)) if index not in scheduled_node_indices]
    if missing_nodes:
        raise ProfileError("selected nodes are absent from generated schedule: " + ", ".join(missing_nodes))
    if not published_names:
        raise ProfileError("selected schedule has no master-published frame")

    raw_id_users: Dict[int, str] = {}
    for frame_name in published_names:
        frame_id = network.frames[frame_name].frame_id
        previous = raw_id_users.get(frame_id)
        if previous is not None and previous != frame_name:
            raise ProfileError(
                "selected profile uses conflicting raw ID 0x{0:02X}: {1} and {2}".format(
                    frame_id, previous, frame_name
                )
            )
        raw_id_users[frame_id] = frame_name
    for node in nodes:
        previous = raw_id_users.get(node["status_frame_id"])
        if previous is not None and previous != node["status_frame"]:
            raise ProfileError(
                "selected profile uses conflicting raw ID 0x{0:02X}: {1} and {2}".format(
                    node["status_frame_id"], previous, node["status_frame"]
                )
            )
        raw_id_users[node["status_frame_id"]] = node["status_frame"]

    control_cfg = config_object(overlay.get("control_signals", {}), "control_signals")
    static_frames = set(config_list(overlay.get("static_master_frames", []), "static_master_frames"))
    if not all(isinstance(item, str) for item in static_frames):
        raise ProfileError("static_master_frames must contain strings")
    unknown_static = static_frames - set(published_names)
    if unknown_static:
        raise ProfileError("static_master_frames are not selected master frames: " + ", ".join(sorted(unknown_static)))

    bindings_by_frame: Dict[str, List[Dict[str, Any]]] = {name: [] for name in published_names}
    for semantic, raw_mapping in control_cfg.items():
        if semantic not in LOGICAL_SIGNAL_ENUMS:
            raise ProfileError(
                "unknown control semantic {0!r}; supported: {1}".format(
                    semantic, ", ".join(LOGICAL_SIGNAL_ENUMS)
                )
            )
        mapping_path = "control_signals." + semantic
        if isinstance(raw_mapping, str):
            mapping_obj: Dict[str, Any] = {"signal": raw_mapping}
        else:
            mapping_obj = config_object(raw_mapping, mapping_path)

        if "signals" in mapping_obj:
            signal_names = config_list(mapping_obj["signals"], mapping_path + ".signals")
        elif "signal" in mapping_obj:
            signal_names = [mapping_obj["signal"]]
        else:
            raise ProfileError("{0} requires signal or signals".format(mapping_path))
        if not signal_names or not all(isinstance(item, str) for item in signal_names):
            raise ProfileError("{0} signal names must be non-empty strings".format(mapping_path))

        frame_name_value = mapping_obj.get("frame")
        if frame_name_value is None:
            candidate = find_signal_frame(network, signal_names[0], published_names)
        else:
            candidate = config_string(frame_name_value, mapping_path + ".frame")
        if candidate not in published_names:
            raise ProfileError("{0} frame is not selected master frame: {1}".format(mapping_path, candidate))
        frame = network.frames[candidate]

        parts: List[Tuple[Signal, FrameSignal]] = [
            frame_mapping(network, frame, signal_name) for signal_name in signal_names
        ]
        expected_start = parts[0][1].start_bit
        combined_default = 0
        total_length = 0
        for signal, placement in parts:
            if placement.start_bit != expected_start + total_length:
                raise ProfileError(
                    "{0} combines non-contiguous signals; current LinSignalLayout can only represent contiguous little-endian parts".format(
                        mapping_path
                    )
                )
            combined_default |= signal.initial << total_length
            total_length += signal.size
        if total_length <= 0 or total_length > 32:
            raise ProfileError("{0} combined width must be 1..32 bits".format(mapping_path))
        if total_length > LOGICAL_STORAGE_BITS[semantic]:
            raise ProfileError(
                "{0} is {1} bits but the application value stores only {2} bits".format(
                    mapping_path, total_length, LOGICAL_STORAGE_BITS[semantic]
                )
            )
        if semantic in BOOLEAN_LOGICAL_SIGNALS and total_length != 1:
            raise ProfileError("{0} must map exactly one bit".format(mapping_path))
        default_value = (
            config_int(mapping_obj["default"], mapping_path + ".default", 0, (1 << total_length) - 1)
            if "default" in mapping_obj
            else combined_default
        )
        bindings_by_frame[candidate].append(
            {
                "semantic": semantic,
                "enum": LOGICAL_SIGNAL_ENUMS[semantic],
                "name": "+".join(signal_names),
                "source_signals": list(signal_names),
                "start_bit": expected_start,
                "bit_length": total_length,
                "default": default_value,
            }
        )

    # Preserve every remaining signal in every selected master-published
    # frame. Unknown business meaning is not permission to discard a field:
    # generic raw bindings keep it editable by exact source name.
    for frame_name in published_names:
        frame = network.frames[frame_name]
        bound_sources = {
            source_name
            for binding in bindings_by_frame[frame_name]
            for source_name in binding.get("source_signals", [])
        }
        for placement in frame.signals:
            if placement.signal in bound_sources:
                continue
            signal = network.signals[placement.signal]
            if signal.size <= 0 or signal.size > 32:
                raise ProfileError(
                    "master signal {0}.{1} has unsupported width {2}; "
                    "all active signals must be represented"
                    .format(frame_name, placement.signal, signal.size)
                )
            bindings_by_frame[frame_name].append(
                {
                    "semantic": "raw",
                    "enum": LOGICAL_SIGNAL_ENUMS["raw"],
                    "name": placement.signal,
                    "source_signals": [placement.signal],
                    "start_bit": placement.start_bit,
                    "bit_length": signal.size,
                    "default": signal.initial,
                }
            )

    payload_overrides = config_object(overlay.get("frame_default_payloads", {}), "frame_default_payloads")
    published_frames: List[Dict[str, Any]] = []
    for frame_name in published_names:
        frame = network.frames[frame_name]
        bindings = bindings_by_frame[frame_name]
        if not bindings and frame_name not in static_frames:
            raise ProfileError(
                "master frame {0} has no business mapping; map a control signal or explicitly acknowledge it in static_master_frames".format(
                    frame_name
                )
            )
        bindings.sort(key=lambda item: item["start_bit"])
        for left, right in zip(bindings, bindings[1:]):
            if left["start_bit"] + left["bit_length"] > right["start_bit"]:
                raise ProfileError(
                    "control mappings {0} and {1} overlap in frame {2}".format(
                        left["semantic"], right["semantic"], frame_name
                    )
                )

        payload = [0] * frame.length
        for placement in frame.signals:
            signal = network.signals[placement.signal]
            write_raw_bits(payload, placement.start_bit, signal.size, signal.initial)
        if frame_name in payload_overrides:
            raw_payload = payload_overrides[frame_name]
            if isinstance(raw_payload, str):
                compact = raw_payload.replace(" ", "").replace("_", "")
                if len(compact) != frame.length * 2 or not re.fullmatch(r"[0-9A-Fa-f]+", compact):
                    raise ProfileError(
                        "frame_default_payloads.{0} must contain exactly {1} hex bytes".format(
                            frame_name, frame.length
                        )
                    )
                payload = [int(compact[i : i + 2], 16) for i in range(0, len(compact), 2)]
            elif isinstance(raw_payload, list) and len(raw_payload) == frame.length:
                payload = [
                    config_int(value, "frame_default_payloads." + frame_name, 0, 255)
                    for value in raw_payload
                ]
            else:
                raise ProfileError(
                    "frame_default_payloads.{0} must be a hex string or byte array of frame length".format(
                        frame_name
                    )
                )
        for binding in bindings:
            write_raw_bits(
                payload, binding["start_bit"], binding["bit_length"], binding["default"]
            )
        published_frames.append(
            {
                "name": frame.name,
                "frame_id": frame.frame_id,
                "length": frame.length,
                "checksum": checksum_name(network, frame.name, overlay),
                "default_payload": payload,
                "bindings": bindings,
                "static": frame.name in static_frames,
            }
        )

    published_index = {frame["name"]: index for index, frame in enumerate(published_frames)}
    if primary_control_frame_name not in published_index:
        raise ProfileError(
            "schedule.control_frame is not a selected master frame: {0}".format(
                primary_control_frame_name
            )
        )
    primary_control_frame_index = published_index[primary_control_frame_name]
    if published_frames[primary_control_frame_index]["static"]:
        raise ProfileError("schedule.control_frame may not be listed in static_master_frames")

    preset_cfg = config_object(overlay.get("signal_presets", {}), "signal_presets")
    preset_items = config_list(preset_cfg.get("items", []), "signal_presets.items")
    if len(preset_items) > 512:
        raise ProfileError("signal_presets.items may contain at most 512 shortcut buttons")
    preset_group_name = ""
    if preset_items:
        preset_group_name = config_string(
            preset_cfg.get("group_name", "Signal Presets"),
            "signal_presets.group_name",
        )
    primary_ldf_frame = network.frames[primary_control_frame_name]
    signal_presets: List[Dict[str, Any]] = []
    used_preset_names: set[str] = set()
    for preset_index, raw_preset in enumerate(preset_items):
        preset_path = "signal_presets.items[{0}]".format(preset_index)
        preset = config_object(raw_preset, preset_path)
        preset_name = config_string(preset.get("name"), preset_path + ".name")
        if preset_name in used_preset_names:
            raise ProfileError("duplicate signal preset name: " + preset_name)
        used_preset_names.add(preset_name)
        values = config_object(preset.get("values"), preset_path + ".values")
        if not values:
            raise ProfileError(preset_path + ".values must not be empty")

        assignments: List[Dict[str, Any]] = []
        for signal_name, raw_value in values.items():
            if not isinstance(signal_name, str) or not signal_name:
                raise ProfileError(preset_path + ".values keys must be signal names")
            signal, placement = frame_mapping(
                network, primary_ldf_frame, signal_name
            )
            if signal.size <= 0 or signal.size > 32:
                raise ProfileError(
                    "{0}.values.{1} uses unsupported signal width {2}; presets support 1..32 bits".format(
                        preset_path, signal_name, signal.size
                    )
                )
            value = config_int(
                raw_value,
                preset_path + ".values." + signal_name,
                0,
                (1 << signal.size) - 1,
            )
            assignments.append(
                {
                    "signal": signal_name,
                    "start_bit": placement.start_bit,
                    "bit_length": signal.size,
                    "value": value,
                }
            )
        assignments.sort(key=lambda item: item["start_bit"])
        signal_presets.append(
            {"name": preset_name, "assignments": assignments}
        )

    for item in schedule_items:
        if item["type"] == "transmit":
            item["table_index"] = published_index[item["frame"]]
        else:
            item["table_index"] = item["node_index"]

    required_semantics = {"target_mask", "intensity"}
    if color_model == "predefined_only":
        required_semantics.add("red_or_predefined")
    elif color_model == "direct_rgb_only":
        required_semantics.update(("red_or_predefined", "green", "blue"))
    else:
        required_semantics.update(
            ("direct_rgb_enable", "red_or_predefined", "green", "blue")
        )
    if addressing_model == "address_value_and_group_bit":
        required_semantics.add("group_addressing")
    if dimming_model == "fade_enable_and_time":
        required_semantics.update(("fading_enable", "fading_time"))
    elif dimming_model == "ramp_with_time_base":
        required_semantics.update(
            ("fading_time", "dimming_time_relative", "dimming_time_base_250ms")
        )
    elif dimming_model == "ramp_time_only":
        required_semantics.add("fading_time")

    primary_semantics = {
        binding["semantic"]
        for binding in published_frames[primary_control_frame_index]["bindings"]
    }
    missing_semantics = required_semantics - primary_semantics
    if missing_semantics:
        raise ProfileError(
            "primary control frame {0} lacks model-required mappings: {1}".format(
                primary_control_frame_name, ", ".join(sorted(missing_semantics))
            )
        )

    primary_bindings = {
        binding["semantic"]: binding
        for binding in published_frames[primary_control_frame_index]["bindings"]
    }
    if intensity_maximum > ((1 << primary_bindings["intensity"]["bit_length"]) - 1):
        raise ProfileError("profile.intensity_maximum does not fit the intensity signal")
    if (
        dimming_model != "none"
        and dimming_maximum
        > ((1 << primary_bindings["fading_time"]["bit_length"]) - 1)
    ):
        raise ProfileError("profile.dimming_maximum does not fit the dimming-time signal")
    if color_model in ("predefined_only", "selectable"):
        color_binding = primary_bindings["red_or_predefined"]
        if len(palette) - 1 > ((1 << color_binding["bit_length"]) - 1):
            raise ProfileError("color.palette indexes do not fit red_or_predefined")

    # Mother-Seed startup policy: publish a useful ON command immediately.
    # Preserve unrelated/static bits, but never require the operator to click
    # once before the selected LEDs receive a non-zero target/intensity/color.
    startup_target = 0
    if addressing_model == "fixed_target_mask":
        for node in nodes:
            startup_target |= node["control_mask"]
    else:
        startup_target = nodes[0]["control_mask"]
    startup_values: Dict[str, int] = {
        "target_mask": startup_target,
        "intensity": intensity_maximum,
        "led_enable": 1,
        "command_validity": 1,
    }
    if color_model in ("direct_rgb_only", "selectable"):
        startup_values["direct_rgb_enable"] = 1
        for semantic in ("red_or_predefined", "green", "blue"):
            if semantic in primary_bindings:
                startup_values[semantic] = (
                    1 << primary_bindings[semantic]["bit_length"]
                ) - 1
    primary_frame = published_frames[primary_control_frame_index]
    for semantic, value in startup_values.items():
        binding = primary_bindings.get(semantic)
        if binding is None:
            continue
        maximum = (1 << binding["bit_length"]) - 1
        if semantic == "intensity":
            value = min(maximum, max(1, value))
        if value < 0 or value > maximum:
            raise ProfileError(
                "visible startup value does not fit primary signal: " + semantic
            )
        binding["default"] = value
        write_raw_bits(
            primary_frame["default_payload"],
            binding["start_bit"],
            binding["bit_length"],
            value,
        )
    if primary_bindings["target_mask"]["default"] == 0:
        raise ProfileError("visible startup target must not be zero")
    if primary_bindings["intensity"]["default"] == 0:
        raise ProfileError("visible startup intensity must not be zero")

    diag_cfg = config_object(overlay.get("diagnostics", {}), "diagnostics")
    request_name: Optional[str]
    response_name: Optional[str]
    if diagnostic_model == "none":
        request_name_value = diag_cfg.get("request_frame")
        response_name_value = diag_cfg.get("response_frame")
        request_name = (
            config_string(request_name_value, "diagnostics.request_frame")
            if request_name_value is not None else None
        )
        response_name = (
            config_string(response_name_value, "diagnostics.response_frame")
            if response_name_value is not None else None
        )
        if request_name is not None and request_name not in network.diagnostic_frames:
            raise ProfileError("diagnostics.request_frame is not a Diagnostic_frames entry")
        if response_name is not None and response_name not in network.diagnostic_frames:
            raise ProfileError("diagnostics.response_frame is not a Diagnostic_frames entry")
        request_default = (
            network.diagnostic_frames[request_name].frame_id
            if request_name is not None else 0x3C
        )
        response_default = (
            network.diagnostic_frames[response_name].frame_id
            if response_name is not None else 0x3D
        )
        request_id = config_int(
            diag_cfg.get("request_frame_id", request_default),
            "diagnostics.request_frame_id", 0, 0x3F,
        )
        response_id = config_int(
            diag_cfg.get("response_frame_id", response_default),
            "diagnostics.response_frame_id", 0, 0x3F,
        )
    else:
        request_name = config_string(
            diag_cfg.get("request_frame"), "diagnostics.request_frame"
        )
        response_name = config_string(
            diag_cfg.get("response_frame"), "diagnostics.response_frame"
        )
        if request_name not in network.diagnostic_frames:
            raise ProfileError("diagnostics.request_frame is not a Diagnostic_frames entry")
        if response_name not in network.diagnostic_frames:
            raise ProfileError("diagnostics.response_frame is not a Diagnostic_frames entry")
        request_id = network.diagnostic_frames[request_name].frame_id
        response_id = network.diagnostic_frames[response_name].frame_id
        for key, actual in (
            ("request_frame_id", request_id), ("response_frame_id", response_id)
        ):
            if key in diag_cfg and config_int(
                diag_cfg[key], "diagnostics." + key, 0, 0x3F
            ) != actual:
                raise ProfileError(
                    "diagnostics.{0} disagrees with the selected LDF frame".format(key)
                )
    if request_id == response_id:
        raise ProfileError("diagnostic request and response raw IDs must differ")
    temporary_nad = config_int(
        diag_cfg.get("temporary_nad", 0 if diagnostic_model == "none" else None),
        "diagnostics.temporary_nad", 0, 255,
    )
    if temporary_nad in {node["nad"] for node in nodes}:
        raise ProfileError("diagnostics.temporary_nad conflicts with an application node NAD")

    services_cfg = config_list(diag_cfg.get("services", []), "diagnostics.services")
    services: List[Dict[str, Any]] = []
    service_ids: set[int] = set()
    nonraw_operations: set[str] = set()
    for index, raw_service in enumerate(services_cfg):
        path = "diagnostics.services[{0}]".format(index)
        service_cfg = config_object(raw_service, path)
        operation = config_string(service_cfg.get("operation"), path + ".operation")
        if operation not in OPERATION_ENUMS:
            raise ProfileError("{0}.operation is unsupported: {1}".format(path, operation))
        if operation != "raw" and operation in nonraw_operations:
            raise ProfileError("diagnostic operation appears more than once: " + operation)
        nonraw_operations.add(operation)
        service_id = config_int(service_cfg.get("service_id"), path + ".service_id", 0, 0xFFFF)
        if service_id in service_ids:
            raise ProfileError("diagnostic service ID appears more than once: 0x{0:04X}".format(service_id))
        service_ids.add(service_id)
        data_length = config_int(service_cfg.get("data_length"), path + ".data_length", 1, 255)
        nad_names = config_list(service_cfg.get("nad_access"), path + ".nad_access")
        nad_access = 0
        for nad_name in nad_names:
            if nad_name == "normal":
                nad_access |= 0x01
            elif nad_name == "calibration":
                nad_access |= 0x02
            else:
                raise ProfileError("{0}.nad_access supports normal/calibration".format(path))
        if nad_access == 0:
            raise ProfileError("{0}.nad_access may not be empty".format(path))
        protocol = config_string(
            service_cfg.get("protocol", "read_by_identifier"), path + ".protocol"
        )
        if protocol not in PROTOCOL_ENUMS:
            raise ProfileError("{0}.protocol is unsupported".format(path))
        readable = config_bool(service_cfg.get("readable", False), path + ".readable")
        writable = config_bool(service_cfg.get("writable", False), path + ".writable")
        read_on_config = config_bool(
            service_cfg.get("read_on_configuration", False), path + ".read_on_configuration"
        )
        positive_write = config_bool(
            service_cfg.get("expect_positive_write_response", False),
            path + ".expect_positive_write_response",
        )
        if not readable and not writable:
            raise ProfileError("{0} must be readable and/or writable".format(path))
        if read_on_config and not readable:
            raise ProfileError("{0} read_on_configuration requires readable=true".format(path))
        if positive_write and not writable:
            raise ProfileError("{0} positive write response requires writable=true".format(path))
        if protocol == "product_identification" and operation != "product_id":
            raise ProfileError("product_identification protocol requires product_id operation")
        services.append(
            {
                "operation": operation,
                "enum": OPERATION_ENUMS[operation],
                "name": require_ascii(
                    config_string(service_cfg.get("name"), path + ".name"), path + ".name"
                ),
                "service_id": service_id,
                "data_length": data_length,
                "nad_access": nad_access,
                "readable": readable,
                "writable": writable,
                "read_on_configuration": read_on_config,
                "expect_positive_write_response": positive_write,
                "protocol": protocol,
                "protocol_enum": PROTOCOL_ENUMS[protocol],
            }
        )

    bulk_cfg = config_list(diag_cfg.get("bulk_write", []), "diagnostics.bulk_write")
    bulk_write: List[Dict[str, str]] = []
    service_by_operation = {
        service["operation"]: service for service in services if service["operation"] != "raw"
    }
    for index, operation_value in enumerate(bulk_cfg):
        operation = config_string(operation_value, "diagnostics.bulk_write[{0}]".format(index))
        service = service_by_operation.get(operation)
        if service is None or not service["writable"]:
            raise ProfileError(
                "diagnostics.bulk_write operation must name a writable non-raw service: {0}".format(
                    operation
                )
            )
        if operation in {item["operation"] for item in bulk_write}:
            raise ProfileError("diagnostics.bulk_write contains duplicate operation " + operation)
        bulk_write.append({"operation": operation, "enum": OPERATION_ENUMS[operation]})

    security_cfg = config_object(diag_cfg.get("security", {}), "diagnostics.security")
    security_enabled = config_bool(security_cfg.get("enabled", False), "diagnostics.security.enabled")
    security = {
        "enabled": security_enabled,
        "request_seed_sub_function": optional_int(
            security_cfg, "request_seed_sub_function", "diagnostics.security", 0, 255, 0
        ),
        "send_key_sub_function": optional_int(
            security_cfg, "send_key_sub_function", "diagnostics.security", 0, 255, 0
        ),
        "key_addend": optional_int(
            security_cfg, "key_addend", "diagnostics.security", 0, 0xFFFFFFFF, 0
        ),
        "key_length": optional_int(
            security_cfg, "key_length", "diagnostics.security", 1, 4, 2
        ),
        "manual_trigger_only": True,
        "lock_service_id": 0x0002,
        "lock_payload": [0x82, 0x00],
    }
    if security_enabled:
        if security["request_seed_sub_function"] == 0 or security["send_key_sub_function"] == 0:
            raise ProfileError("enabled diagnostic security requires nonzero seed/key sub-functions")
        if security["request_seed_sub_function"] == security["send_key_sub_function"]:
            raise ProfileError("diagnostic seed/key sub-functions must differ")
        if security["key_addend"] != 0x0C04 or security["key_length"] != 4:
            raise ProfileError(
                "mother-Seed SecurityAccess requires key_addend=0x0C04 "
                "and key_length=4"
            )

    if diagnostic_model == "custom_did":
        if not services:
            raise ProfileError("models.diagnostic=custom_did requires diagnostics.services")
    else:
        if services or bulk_write:
            raise ProfileError(
                "only models.diagnostic=custom_did may define proprietary services or bulk_write"
            )
        if security_enabled:
            raise ProfileError(
                "diagnostic security is only supported by models.diagnostic=custom_did"
            )

    warnings = list(network.warnings)
    if delay_mode == "copy_as_post_delay":
        warnings.append(
            "LDF schedule slot times are emitted as LinScheduleSlot.delayAfterMs; the current worker waits after transaction completion, so the real cycle is conservatively longer than the LDF slot table"
        )
    if any(node["nad_source"] == "overlay" and node["ldf_nad"] != node["nad"] for node in nodes):
        warnings.append("one or more effective NAD values override LDF Node_attributes; verify against slave firmware")

    return {
        "generator_version": GENERATOR_VERSION,
        "source": {
            "name": network.source_name,
            "encoding": network.source_encoding,
            "sha256": network.source_sha256,
            "overlay_sha256": overlay_sha256,
        },
        "layout_name": layout_name,
        "device_name": device_name,
        "baud_rate": network.baud_rate,
        "serial_io_timeout_ms": serial_timeout,
        "models": {
            "color": color_model,
            "color_enum": COLOR_MODEL_ENUMS[color_model],
            "addressing": addressing_model,
            "addressing_enum": ADDRESSING_MODEL_ENUMS[addressing_model],
            "dimming": dimming_model,
            "dimming_enum": DIMMING_MODEL_ENUMS[dimming_model],
            "diagnostic": diagnostic_model,
            "diagnostic_enum": DIAGNOSTIC_MODEL_ENUMS[diagnostic_model],
        },
        "intensity_maximum": intensity_maximum,
        "dimming_maximum": dimming_maximum,
        "startup_output_policy": "visible_on_power_up",
        "predefined_colors": palette,
        "signal_preset_group_name": preset_group_name,
        "signal_presets": signal_presets,
        "published_frames": published_frames,
        "primary_control_frame": primary_control_frame_name,
        "primary_control_frame_index": primary_control_frame_index,
        "status_layouts": status_layouts,
        "nodes": nodes,
        "diagnostics": {
            "request_frame": request_name,
            "request_frame_id": request_id,
            "response_frame": response_name,
            "response_frame_id": response_id,
            "temporary_nad": temporary_nad,
            "services": services,
            "bulk_write": bulk_write,
            "security": security,
        },
        "schedule": {
            "source_table": schedule_name,
            "delay_mode": delay_mode,
            "master_frame_policy": master_policy,
            "items": schedule_items,
            "skipped": skipped,
        },
        "ldf_timing_minimums": {
            "p2_ms": ldf_p2_minimum,
            "st_ms": ldf_st_minimum,
        },
        "diagnostic_request_to_response_delay_ms": diagnostic_request_delay,
        "diagnostic_inter_frame_delay_ms": diag_gap,
        "response_delay_ms": response_delay,
        "post_write_settle_ms": post_write,
        "bulk_write_readback_delay_ms": bulk_readback_delay,
        "maximum_diagnostic_queue_depth": max_queue,
        "warnings": warnings,
    }


def cpp_string(value: str) -> str:
    require_ascii(value, "generated C++ string")
    return '"' + value.replace("\\", "\\\\").replace('"', '\\"') + '"'


def cpp_utf8_string(value: str) -> str:
    """Emit a compiler-independent UTF-8 byte string for QString::fromUtf8."""
    return '"' + "".join("\\x{0:02X}".format(byte) for byte in value.encode("utf-8")) + '"'


def cpp_bool(value: bool) -> str:
    return "true" if value else "false"


def cpp_hex(value: int, width: int = 2) -> str:
    return "0x{0:0{1}X}".format(value, width)


def symbol(value: str) -> str:
    cleaned = re.sub(r"[^A-Za-z0-9_]", "_", value)
    if not cleaned or cleaned[0].isdigit():
        cleaned = "_" + cleaned
    return cleaned


def emit_header(profile: Mapping[str, Any]) -> str:
    return """// Generated by tools/ldf_profile_gen.py {version}.
// Source: {source} (sha256 {sha})
// Do not edit this file; edit the LDF or JSON overlay and regenerate.

#ifndef LINPROFILE_GENERATED_H
#define LINPROFILE_GENERATED_H

struct LinLayout;

const LinLayout &generatedLinProfile();

#endif // LINPROFILE_GENERATED_H
""".format(
        version=GENERATOR_VERSION,
        source=profile["source"]["name"],
        sha=profile["source"]["sha256"],
    )


def emit_source(profile: Mapping[str, Any]) -> str:
    lines: List[str] = [
        "// Generated by tools/ldf_profile_gen.py {0}.".format(GENERATOR_VERSION),
        "// Source: {0} (sha256 {1})".format(
            profile["source"]["name"], profile["source"]["sha256"]
        ),
        "// Overlay sha256: {0}".format(profile["source"]["overlay_sha256"]),
        "// Do not edit this file; edit the LDF or JSON overlay and regenerate.",
        "",
        '#include "linprofile_generated.h"',
        '#include "linlayout.h"',
        "",
        "namespace {",
        "",
    ]

    frame_signal_symbols: List[Optional[str]] = []
    for index, frame in enumerate(profile["published_frames"]):
        bindings = frame["bindings"]
        if not bindings:
            frame_signal_symbols.append(None)
            continue
        name = "kPublishedFrame{0}Signals".format(index)
        frame_signal_symbols.append(name)
        lines.append("static const LinSignalLayout {0}[] = {{".format(name))
        for binding in bindings:
            lines.append(
                "  {{{enum}, {name}, {start}, {length}, {default_value}}},".format(
                    enum=binding["enum"],
                    name=cpp_string(binding["name"]),
                    start=binding["start_bit"],
                    length=binding["bit_length"],
                    default_value=cpp_hex(binding["default"], max(2, (binding["bit_length"] + 3) // 4)),
                )
            )
        lines.extend(("};", ""))

    lines.append("static const LinFrameLayout kPublishedFrames[] = {")
    for index, frame in enumerate(profile["published_frames"]):
        payload = list(frame["default_payload"]) + [0] * (8 - len(frame["default_payload"]))
        signal_symbol = frame_signal_symbols[index]
        lines.extend(
            [
                "  {",
                "    {0},".format(cpp_string(frame["name"])),
                "    {0},".format(cpp_hex(frame["frame_id"])),
                "    {0},".format(frame["length"]),
                "    {0},".format(CHECKSUM_ENUMS[frame["checksum"]]),
                "    {{{0}}},".format(", ".join(cpp_hex(value) for value in payload)),
                "    {0},".format(signal_symbol or "0"),
                "    {0}".format(len(frame["bindings"])),
                "  },",
            ]
        )
    lines.extend(("};", ""))

    status_field_symbols: List[Optional[str]] = []
    for index, layout in enumerate(profile["status_layouts"]):
        fields = layout["fields"]
        if not fields:
            status_field_symbols.append(None)
            continue
        name = "kStatusLayout{0}Fields".format(index)
        status_field_symbols.append(name)
        lines.append("static const LinStatusFieldLayout {0}[] = {{".format(name))
        for item in fields:
            lines.append(
                "  {{{0}, {1}, {2}, {3}, {4}, {5}, {6}}},".format(
                    item["enum"],
                    cpp_string(item["name"]),
                    item["start_bit"],
                    item["bit_length"],
                    cpp_hex(item["normal_value"], max(2, (item["bit_length"] + 3) // 4)),
                    cpp_hex(item["error_value"], max(2, (item["bit_length"] + 3) // 4)),
                    cpp_bool(item["any_non_normal_is_error"]),
                )
            )
        lines.extend(("};", ""))

    lines.append("static const LinStatusLayout kStatusLayouts[] = {")
    for index, layout in enumerate(profile["status_layouts"]):
        lines.append(
            "  {{{0}, {1}, {2}}},".format(
                cpp_string(layout["name"]),
                status_field_symbols[index] or "0",
                len(layout["fields"]),
            )
        )
    lines.extend(("};", ""))

    if profile["predefined_colors"]:
        lines.append("static const LinPredefinedColor kPredefinedColors[] = {")
        for red, green, blue in profile["predefined_colors"]:
            lines.append(
                "  {{{0}, {1}, {2}}},".format(cpp_hex(red), cpp_hex(green), cpp_hex(blue))
            )
        lines.extend(("};", ""))

    preset_assignment_symbols: List[str] = []
    for preset_index, preset in enumerate(profile["signal_presets"]):
        name = "kSignalPreset{0}Assignments".format(preset_index)
        preset_assignment_symbols.append(name)
        lines.append("static const LinSignalPresetAssignment {0}[] = {{".format(name))
        for assignment in preset["assignments"]:
            lines.append(
                "  {{{signal}, {start}, {length}, {value}}},".format(
                    signal=cpp_string(assignment["signal"]),
                    start=assignment["start_bit"],
                    length=assignment["bit_length"],
                    value=cpp_hex(
                        assignment["value"],
                        max(2, (assignment["bit_length"] + 3) // 4),
                    ),
                )
            )
        lines.extend(("};", ""))

    if profile["signal_presets"]:
        lines.append("static const LinSignalPreset kSignalPresets[] = {")
        for preset_index, preset in enumerate(profile["signal_presets"]):
            lines.append(
                "  {{{name}, {assignments}, {count}}},".format(
                    name=cpp_utf8_string(preset["name"]),
                    assignments=preset_assignment_symbols[preset_index],
                    count=len(preset["assignments"]),
                )
            )
        lines.extend(("};", ""))

    lines.append("static const LinNodeLayout kNodes[] = {")
    for node in profile["nodes"]:
        lines.append(
            "  {{{ui}, {nad}, {mask}, {frame}, {length}, {layout}, {checksum}, {node_type}, {name}}},".format(
                ui=node["ui_slot"],
                nad=cpp_hex(node["nad"]),
                mask=cpp_hex(node["control_mask"], 4),
                frame=cpp_hex(node["status_frame_id"]),
                length=node["status_length"],
                layout=node["status_layout_index"],
                checksum=CHECKSUM_ENUMS[node["checksum"]],
                node_type=NODE_TYPE_ENUMS[node["node_type"]],
                name=cpp_string(node["name"]),
            )
        )
    lines.extend(("};", ""))

    services = profile["diagnostics"]["services"]
    if services:
        lines.append("static const LinServiceLayout kServices[] = {")
        for service in services:
            lines.append(
                "  {{{operation}, {name}, {service_id}, {length}, {nad}, {readable}, {writable}, {read_config}, {positive}, {protocol}}},".format(
                    operation=service["enum"],
                    name=cpp_string(service["name"]),
                    service_id=cpp_hex(service["service_id"], 4),
                    length=service["data_length"],
                    nad=cpp_hex(service["nad_access"]),
                    readable=cpp_bool(service["readable"]),
                    writable=cpp_bool(service["writable"]),
                    read_config=cpp_bool(service["read_on_configuration"]),
                    positive=cpp_bool(service["expect_positive_write_response"]),
                    protocol=service["protocol_enum"],
                )
            )
        lines.extend(("};", ""))

    bulk = profile["diagnostics"]["bulk_write"]
    if bulk:
        lines.append("static const OperationType kBulkWriteSequence[] = {")
        for item in bulk:
            lines.append("  {0},".format(item["enum"]))
        lines.extend(("};", ""))

    lines.append("static const LinScheduleSlot kSchedule[] = {")
    for item in profile["schedule"]["items"]:
        slot_enum = "ELinScheduleTransmitFrame" if item["type"] == "transmit" else "ELinSchedulePollNode"
        lines.append(
            "  {{{0}, {1}, {2}}}, // {3}".format(
                slot_enum, item["table_index"], item["delay_after_ms"], item["frame"]
            )
        )
    lines.extend(("};", ""))

    palette_ptr = "kPredefinedColors" if profile["predefined_colors"] else "0"
    preset_ptr = "kSignalPresets" if profile["signal_presets"] else "0"
    service_ptr = "kServices" if services else "0"
    bulk_ptr = "kBulkWriteSequence" if bulk else "0"
    security = profile["diagnostics"]["security"]
    lines.extend(
        [
            "static const LinLayout kGeneratedLayout = {",
            "  {0},".format(cpp_string(profile["layout_name"])),
            "  {0},".format(cpp_string(profile["device_name"])),
            "  {0},".format(profile["baud_rate"]),
            "  {0},".format(profile["serial_io_timeout_ms"]),
            "  {0},".format(profile["models"]["color_enum"]),
            "  {0},".format(profile["models"]["addressing_enum"]),
            "  {0},".format(profile["models"]["dimming_enum"]),
            "  {0},".format(profile["models"]["diagnostic_enum"]),
            "  {0},".format(profile["intensity_maximum"]),
            "  {0},".format(profile["dimming_maximum"]),
            "  {0},".format(palette_ptr),
            "  {0},".format(len(profile["predefined_colors"])),
            "  {0},".format(
                cpp_utf8_string(profile["signal_preset_group_name"])
                if profile["signal_presets"] else "0"
            ),
            "  {0},".format(preset_ptr),
            "  {0},".format(len(profile["signal_presets"])),
            "  kPublishedFrames,",
            "  {0},".format(len(profile["published_frames"])),
            "  {0},".format(profile["primary_control_frame_index"]),
            "  kStatusLayouts,",
            "  {0},".format(len(profile["status_layouts"])),
            "  kNodes,",
            "  {0},".format(len(profile["nodes"])),
            "  {0},".format(cpp_hex(profile["diagnostics"]["request_frame_id"])),
            "  {0},".format(cpp_hex(profile["diagnostics"]["response_frame_id"])),
            "  {0},".format(cpp_hex(profile["diagnostics"]["temporary_nad"])),
            "  {0},".format(service_ptr),
            "  {0},".format(len(services)),
            "  {0},".format(bulk_ptr),
            "  {0},".format(len(bulk)),
            "  {{{0}, {1}, {2}, {3}, {4}}},".format(
                cpp_bool(security["enabled"]),
                cpp_hex(security["request_seed_sub_function"]),
                cpp_hex(security["send_key_sub_function"]),
                cpp_hex(security["key_addend"], 8),
                security["key_length"],
            ),
            "  kSchedule,",
            "  {0},".format(len(profile["schedule"]["items"])),
            "  {0},".format(profile["diagnostic_request_to_response_delay_ms"]),
            "  {0},".format(profile["diagnostic_inter_frame_delay_ms"]),
            "  {0},".format(profile["response_delay_ms"]),
            "  {0},".format(profile["post_write_settle_ms"]),
            "  {0},".format(profile["bulk_write_readback_delay_ms"]),
            "  {0}".format(profile["maximum_diagnostic_queue_depth"]),
            "};",
            "",
            "} // namespace",
            "",
            "const LinLayout &generatedLinProfile()",
            "{",
            "  return kGeneratedLayout;",
            "}",
            "",
        ]
    )
    return "\n".join(lines)


def make_report(network: LdfNetwork, profile: Dict[str, Any]) -> Dict[str, Any]:
    all_frames = []
    for frame in network.frames.values():
        all_frames.append(
            {
                "name": frame.name,
                "raw_id": "0x{0:02X}".format(frame.frame_id),
                "publisher": frame.publisher,
                "length": frame.length,
                "signals": [
                    {
                        "name": placement.signal,
                        "start_bit": placement.start_bit,
                        "bit_length": network.signals[placement.signal].size,
                        "initial": network.signals[placement.signal].initial,
                    }
                    for placement in frame.signals
                ],
            }
        )
    generated = json.loads(json.dumps(profile))
    for frame in generated["published_frames"]:
        frame["raw_id"] = "0x{0:02X}".format(frame.pop("frame_id"))
        frame["default_payload_hex"] = " ".join(
            "{0:02X}".format(value) for value in frame.pop("default_payload")
        )
    for node in generated["nodes"]:
        node["nad_hex"] = "0x{0:02X}".format(node["nad"])
        node["control_mask_hex"] = "0x{0:04X}".format(node["control_mask"])
        node["status_raw_id"] = "0x{0:02X}".format(node["status_frame_id"])
    for service in generated["diagnostics"]["services"]:
        service["service_id_hex"] = "0x{0:04X}".format(service["service_id"])

    return {
        "schema_version": 1,
        "source_ldf": {
            "name": network.source_name,
            "encoding": network.source_encoding,
            "sha256": network.source_sha256,
            "protocol_version": network.protocol_version,
            "language_version": network.language_version,
            "baud_rate": network.baud_rate,
            "master": network.master,
            "slaves": network.slaves,
        },
        "node_attributes": {
            name: {
                "configured_nad": attributes.configured_nad,
                "initial_nad": attributes.initial_nad,
                "lin_protocol": attributes.protocol,
                "product_id": list(attributes.product_id),
                "response_error": attributes.response_error,
                "p2_min_ms": attributes.p2_min_ms,
                "st_min_ms": attributes.st_min_ms,
            }
            for name, attributes in network.node_attributes.items()
        },
        "available_schedule_tables": {
            name: [
                {"frame": entry.frame, "delay_ms": entry.delay_ms}
                for entry in table.entries
            ]
            for name, table in network.schedules.items()
        },
        "parsed_frames": all_frames,
        "generated_profile": generated,
        "integration": {
            "ready_for_linlayout_types": True,
            "ambientdevice_modified_by_generator": False,
            "todo": [
                "Add the fixed generated .cpp/.h to AmbientDevice.pro once.",
                "Make defaultLinLayout() return generatedLinProfile() once profile selection is enabled.",
            ],
        },
    }


def render_outputs(network: LdfNetwork, overlay: Dict[str, Any], overlay_sha256: str) -> Dict[str, str]:
    profile = build_profile(network, overlay, overlay_sha256)
    report = make_report(network, profile)
    return {
        OUTPUT_HEADER: emit_header(profile),
        OUTPUT_SOURCE: emit_source(profile),
        OUTPUT_REPORT: json.dumps(report, ensure_ascii=False, indent=2, sort_keys=False) + "\n",
    }


def atomic_write_text(path: Path, content: str) -> None:
    # Keep both rename operands absolute.  Windows shared-folder drivers may
    # otherwise report WinError 17 even when the relative destination is on
    # the same mounted drive as the temporary file.
    path = path.resolve()
    path.parent.mkdir(parents=True, exist_ok=True)
    descriptor, temporary_name = tempfile.mkstemp(
        prefix=path.name + ".", suffix=".tmp", dir=str(path.parent)
    )
    try:
        with os.fdopen(descriptor, "w", encoding="utf-8", newline="\n") as stream:
            stream.write(content)
        try:
            os.replace(temporary_name, path)
        except OSError:
            # Some Windows/VM shared-folder drivers cannot replace an
            # existing C/C++ file, even when both paths are on the same
            # mount.  Generation output is reproducible, so fall back to a
            # direct write after the preferred atomic replace fails.
            if not os.path.exists(temporary_name):
                raise
            with path.open("w", encoding="utf-8", newline="\n") as stream:
                stream.write(content)
            os.unlink(temporary_name)
    except Exception:
        try:
            os.unlink(temporary_name)
        except OSError:
            pass
        raise


def status_frame_candidates(network: LdfNetwork, slave: str) -> List[str]:
    return [frame.name for frame in network.frames.values() if frame.publisher == slave]


def make_initial_overlay(network: LdfNetwork, ldf_path: Path) -> Dict[str, Any]:
    normal_schedules = [
        name
        for name, table in network.schedules.items()
        if not any(entry.frame in network.diagnostic_frames for entry in table.entries)
    ]
    master_frames = [frame.name for frame in network.frames.values() if frame.publisher == network.master]
    master_signals = [
        signal.name for signal in network.signals.values() if signal.publisher == network.master
    ]
    nodes = []
    for index, slave in enumerate(network.slaves):
        attributes = network.node_attributes.get(slave)
        nad = None if attributes is None else (
            attributes.initial_nad
            if attributes.initial_nad is not None
            else attributes.configured_nad
        )
        candidates = status_frame_candidates(network, slave)
        nodes.append(
            {
                "ldf_node": slave,
                "name": slave,
                "nad": "TODO_VERIFY_NAD" if nad is None or not 1 <= nad <= 0x7D else "0x{0:02X}".format(nad),
                "control_mask": "TODO_ONE_HOT_MASK",
                "ui_slot": index,
                "node_type": "TODO_rgb_OR_white",
                "status_frame": candidates[0] if len(candidates) == 1 else "TODO_SELECT_STATUS_FRAME",
                "_status_frame_candidates": candidates,
            }
        )

    request_candidate = "MasterReq" if "MasterReq" in network.diagnostic_frames else "TODO_SELECT_REQUEST_FRAME"
    response_candidate = "SlaveResp" if "SlaveResp" in network.diagnostic_frames else "TODO_SELECT_RESPONSE_FRAME"
    return {
        "schema_version": 1,
        "_generated_from": ldf_path.name,
        "_review": [
            "Every TODO must be resolved before generate/check.",
            "Remove nodes and scheduled frames that do not belong to this application profile explicitly.",
            "LDF signal presence does not prove firmware support for predefined/direct RGB.",
        ],
        "profile": {
            "layout_name": ldf_path.stem,
            "device_name": "/dev/ttySAC3",
            "serial_io_timeout_ms": 100,
            "intensity_maximum": "TODO_MATCH_SIGNAL_AND_UI_RANGE",
            "dimming_maximum": "TODO_MATCH_SIGNAL_AND_UI_RANGE",
            "diagnostic_request_to_response_delay_ms": "TODO_AT_LEAST_LDF_P2_MIN",
            "diagnostic_inter_frame_delay_ms": "TODO_AT_LEAST_LDF_ST_MIN",
            "response_delay_ms": 10,
            "post_write_settle_ms": 100,
            "bulk_write_readback_delay_ms": 1000,
            "maximum_diagnostic_queue_depth": 16,
        },
        "models": {
            "color": "TODO_predefined_only_OR_direct_rgb_only_OR_selectable",
            "addressing": "TODO_fixed_target_mask_OR_address_value_and_group_bit",
            "dimming": "TODO_fade_enable_and_time_OR_ramp_with_time_base",
            "diagnostic": "TODO_none_OR_lin20_node_configuration_OR_custom_did",
        },
        "schedule": {
            "table": normal_schedules[0] if len(normal_schedules) == 1 else "TODO_SELECT_SCHEDULE",
            "control_frame": master_frames[0] if len(master_frames) == 1 else "TODO_SELECT_CONTROL_FRAME",
            "delay_mode": "TODO_copy_as_post_delay_OR_fixed_post_delay",
            "master_frame_policy": "preserve",
            "ignored_frames": [],
            "_candidates": normal_schedules,
        },
        "nodes": nodes,
        "control_signals": {},
        "_control_signal_candidates": {
            "master_frames": master_frames,
            "master_signals": master_signals,
            "supported_semantics": list(LOGICAL_SIGNAL_ENUMS),
        },
        "status_signal_map": {},
        "status_suffix_map": {},
        "allow_empty_status": False,
        "static_master_frames": [],
        "frame_default_payloads": {},
        "checksum_overrides": {},
        "color": {"palette": []},
        "signal_presets": {
            "group_name": "Signal Presets",
            "items": [],
        },
        "diagnostics": {
            "request_frame": request_candidate,
            "response_frame": response_candidate,
            "temporary_nad": "0xA0",
            "services": [],
            "bulk_write": [],
            "security": {
                "enabled": False,
                "request_seed_sub_function": "0x00",
                "send_key_sub_function": "0x00",
                "key_addend": "0x0000",
                "key_length": 2,
            },
            "_diagnostic_frame_candidates": list(network.diagnostic_frames),
        },
    }


def command_init(args: argparse.Namespace) -> int:
    ldf_path = Path(args.ldf)
    overlay_path = Path(args.overlay)
    network = parse_ldf(ldf_path)
    if overlay_path.exists() and not args.force:
        raise ProfileError(
            "overlay already exists: {0}; use --force only if replacing the human mapping is intentional".format(
                overlay_path
            )
        )
    content = json.dumps(
        make_initial_overlay(network, ldf_path), ensure_ascii=False, indent=2
    ) + "\n"
    atomic_write_text(overlay_path, content)
    print(
        "created {0}: {1} signals, {2} frames, {3} nodes, schedules [{4}]".format(
            overlay_path,
            len(network.signals),
            len(network.frames),
            len(network.slaves),
            ", ".join(network.schedules),
        )
    )
    return 0


def command_generate(args: argparse.Namespace) -> int:
    network = parse_ldf(Path(args.ldf))
    overlay, overlay_sha = load_overlays([Path(path) for path in args.overlay])
    outputs = render_outputs(network, overlay, overlay_sha)
    output_dir = Path(args.output)
    for name, content in outputs.items():
        atomic_write_text(output_dir / name, content)
    generated = json.loads(outputs[OUTPUT_REPORT])["generated_profile"]
    print(
        "generated {0}: {1} published frames, {2} nodes, {3} schedule slots".format(
            output_dir,
            len(generated["published_frames"]),
            len(generated["nodes"]),
            len(generated["schedule"]["items"]),
        )
    )
    return 0


def command_check(args: argparse.Namespace) -> int:
    network = parse_ldf(Path(args.ldf))
    overlay, overlay_sha = load_overlays([Path(path) for path in args.overlay])
    outputs = render_outputs(network, overlay, overlay_sha)
    output_dir = Path(args.output)
    differences: List[str] = []
    for name, expected in outputs.items():
        path = output_dir / name
        try:
            actual = path.read_text(encoding="utf-8")
        except OSError:
            differences.append(name + " (missing)")
            continue
        if actual.replace("\r\n", "\n") != expected:
            differences.append(name + " (out of date)")
    if differences:
        print("generated profile check failed: " + ", ".join(differences), file=sys.stderr)
        return 1
    print("generated profile is up to date: " + str(output_dir))
    return 0


def build_argument_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(
        description="Generate AmbientDevice LinLayout C++ data from LDF + JSON semantics."
    )
    subparsers = parser.add_subparsers(dest="command", required=True)

    init_parser = subparsers.add_parser(
        "init", help="parse an LDF and create a review-required JSON overlay template"
    )
    init_parser.add_argument("--ldf", required=True, help="input .ldf path")
    init_parser.add_argument("--overlay", required=True, help="new JSON overlay path")
    init_parser.add_argument("--force", action="store_true", help="replace an existing overlay")
    init_parser.set_defaults(handler=command_init)

    for name, help_text, handler in (
        ("generate", "validate and write fixed-name C++/JSON outputs", command_generate),
        ("check", "validate and compare fixed-name outputs without writing", command_check),
    ):
        subparser = subparsers.add_parser(name, help=help_text)
        subparser.add_argument("--ldf", required=True, help="input .ldf path")
        subparser.add_argument(
            "--overlay",
            required=True,
            action="append",
            help="JSON overlay; repeat to merge common then profile-specific semantics",
        )
        subparser.add_argument("--output", required=True, help="output directory")
        subparser.set_defaults(handler=handler)

    return parser


def main(argv: Optional[Sequence[str]] = None) -> int:
    parser = build_argument_parser()
    args = parser.parse_args(argv)
    try:
        return int(args.handler(args))
    except ProfileError as exc:
        print("error: {0}".format(exc), file=sys.stderr)
        return 2
    except OSError as exc:
        print("error: filesystem operation failed: {0}".format(exc), file=sys.stderr)
        return 2


if __name__ == "__main__":
    sys.exit(main())
