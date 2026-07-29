#!/usr/bin/env python3
"""Build an evidence-backed LIN contract draft from slave C sources only."""

from __future__ import annotations

import argparse
import json
import re
import sys
from pathlib import Path
from typing import Dict, Iterable, List, Optional, Sequence, Tuple


IGNORED_DIRS = {
    ".git",
    "__pycache__",
    "build",
    "debug",
    "release",
    "objects",
    "listings",
}
SOURCE_SUFFIXES = {".c", ".h", ".inc"}
NUMBER = r"(?:0[xX][0-9A-Fa-f]+|\d+)"


def parse_int(value: str) -> int:
    return int(re.sub(r"[uUlL]+$", "", value.strip()), 0)


def read_source(path: Path) -> str:
    raw = path.read_bytes()
    for encoding in ("utf-8-sig", "cp1252", "latin-1"):
        try:
            return raw.decode(encoding)
        except UnicodeDecodeError:
            continue
    raise ValueError("cannot decode source")


def source_files(root: Path) -> Iterable[Path]:
    for path in sorted(root.rglob("*")):
        if not path.is_file() or path.suffix.lower() not in SOURCE_SUFFIXES:
            continue
        relative_parts = [part.lower() for part in path.relative_to(root).parts[:-1]]
        if any(part in IGNORED_DIRS or part.startswith("build-") for part in relative_parts):
            continue
        yield path


def evidence(root: Path, path: Path, line: int, text: str) -> Dict[str, object]:
    return {
        "file": path.relative_to(root).as_posix(),
        "line": line,
        "contains": text.strip()[:240],
    }


def logical_macros(text: str) -> Iterable[Tuple[int, str]]:
    lines = text.splitlines()
    index = 0
    while index < len(lines):
        start = index
        current = lines[index]
        while current.rstrip().endswith("\\") and index + 1 < len(lines):
            index += 1
            current = current.rstrip()[:-1] + " " + lines[index].strip()
        if current.lstrip().startswith("#define"):
            yield start + 1, current
        index += 1


def contiguous_width(mask: int) -> Optional[int]:
    if mask <= 0:
        return None
    width = mask.bit_length()
    return width if mask == (1 << width) - 1 else None


def parse_signal_accessors(
    root: Path, path: Path, text: str
) -> List[Dict[str, object]]:
    result: List[Dict[str, object]] = []
    read_pattern = re.compile(
        r"#define\s+l_(?:bool|u\d+)_rd_(?P<signal>\w+)\(\).*?"
        r"l_LinData\.frames\.l_frm_(?P<frame>\w+)\.frame_data"
        r"\[(?P<byte>\d+)\](?:\s*\)\s*|\s*)"
        r"(?:>>\s*(?P<shift>\d+)u?)?.*?&\s*(?P<mask>" + NUMBER + r")u?",
        re.IGNORECASE,
    )
    write_pattern = re.compile(
        r"#define\s+l_(?:bool|u\d+)_wr_(?P<signal>\w+)\([^)]*\).*?"
        r"l_LinData\.frames\.l_frm_(?P<frame>\w+)\.frame_data"
        r"\[(?P<byte>\d+)\]\s*&=\s*\((?P<clear>" + NUMBER + r")u?\).*?"
        r"(?:<<\s*(?P<shift>\d+)u?)?",
        re.IGNORECASE,
    )
    for line, macro in logical_macros(text):
        match = read_pattern.search(macro)
        if match:
            mask = parse_int(match.group("mask"))
            shift = int(match.group("shift") or 0)
            width = contiguous_width(mask)
            result.append(
                {
                    "frame": match.group("frame"),
                    "direction": "master_to_slave",
                    "signal": match.group("signal"),
                    "start_bit": int(match.group("byte")) * 8 + shift,
                    "bit_length": width,
                    "raw_mask": mask,
                    "evidence": [evidence(root, path, line, macro)],
                }
            )
            continue
        match = write_pattern.search(macro)
        if match:
            clear = parse_int(match.group("clear")) & 0xFF
            signal_mask = (~clear) & 0xFF
            shift = (
                int(match.group("shift"))
                if match.group("shift") is not None
                else (signal_mask & -signal_mask).bit_length() - 1
            )
            width = contiguous_width(signal_mask >> shift)
            result.append(
                {
                    "frame": match.group("frame"),
                    "direction": "slave_to_master",
                    "signal": match.group("signal"),
                    "start_bit": int(match.group("byte")) * 8 + shift,
                    "bit_length": width,
                    "raw_mask": signal_mask,
                    "evidence": [evidence(root, path, line, macro)],
                }
            )
    return result


def parse_service_rows(
    root: Path, path: Path, text: str
) -> List[Dict[str, object]]:
    row_pattern = re.compile(
        r"\{\s*(?P<did>" + NUMBER + r")u?\s*,\s*"
        r"(?P<access>E_DS_\w+)\s*,\s*(?P<nad>E_NAD_\w+)\s*,\s*"
        r"(?P<length>" + NUMBER + r"|[A-Za-z_]\w*)[uUlL]*\s*,"
    )
    rows: List[Dict[str, object]] = []
    for line_no, line in enumerate(text.splitlines(), 1):
        match = row_pattern.search(line)
        if not match:
            continue
        length_token = match.group("length")
        rows.append(
            {
                "did": parse_int(match.group("did")),
                "did_hex": "0x{0:04X}".format(parse_int(match.group("did"))),
                "access": match.group("access"),
                "nad_scope": match.group("nad"),
                "length": (
                    parse_int(length_token)
                    if re.fullmatch(NUMBER + r"[uUlL]*", length_token)
                    else length_token
                ),
                "evidence": [evidence(root, path, line_no, line)],
            }
        )
    return rows


def parse_pair_tables(
    root: Path, path: Path, text: str
) -> List[Dict[str, object]]:
    tables: List[Dict[str, object]] = []
    table_pattern = re.compile(
        r"(?:static\s+)?(?:const\s+)?(?:u?int(?:8|16)_t|unsigned\s+char)"
        r"\s+(?P<name>\w*(?:nad|pid|addr)\w*)\s*"
        r"\[[^\]]+\]\s*\[[^\]]+\]\s*=\s*\{(?P<body>.*?)\};",
        re.IGNORECASE | re.DOTALL,
    )
    pair_pattern = re.compile(
        r"\{\s*(?P<a>" + NUMBER + r")\s*,\s*(?P<b>" + NUMBER + r")\s*\}"
    )
    for match in table_pattern.finditer(text):
        pairs = [
            {
                "input": parse_int(pair.group("a")),
                "output": parse_int(pair.group("b")),
            }
            for pair in pair_pattern.finditer(match.group("body"))
        ]
        if not pairs:
            continue
        line = text.count("\n", 0, match.start()) + 1
        tables.append(
            {
                "name": match.group("name"),
                "pairs": pairs,
                "meaning": "review_required",
                "evidence": [
                    evidence(root, path, line, match.group(0).splitlines()[0])
                ],
            }
        )
    return tables


def scan(root: Path) -> Dict[str, object]:
    texts: Dict[Path, str] = {}
    decode_errors: List[str] = []
    for path in source_files(root):
        try:
            texts[path] = read_source(path)
        except ValueError:
            decode_errors.append(path.relative_to(root).as_posix())

    signals: List[Dict[str, object]] = []
    services: List[Dict[str, object]] = []
    pair_tables: List[Dict[str, object]] = []
    nad_evidence: List[Dict[str, object]] = []
    diagnostic_evidence: List[Dict[str, object]] = []
    sid_values: Dict[int, List[Dict[str, object]]] = {}
    diag_enabled = False

    nad_line = re.compile(
        r"\bNAD\b|Initial_NAD|current_NAD|SlaveNodeNAD|E_NAD_|"
        r"LIN_(?:BROADCAST|FUNCTIONAL)_NAD",
        re.IGNORECASE,
    )
    sid_line = re.compile(
        r"(?:MasterReq|SID|Ssid|service)[^;\n]{0,100}?(0[xX][0-9A-Fa-f]+)",
        re.IGNORECASE,
    )

    for path, text in texts.items():
        signals.extend(parse_signal_accessors(root, path, text))
        services.extend(parse_service_rows(root, path, text))
        pair_tables.extend(parse_pair_tables(root, path, text))
        for line_no, line in enumerate(text.splitlines(), 1):
            if nad_line.search(line):
                nad_evidence.append(evidence(root, path, line_no, line))
            if "LIN_DIAG_ENABLE" in line or "LIN_USER_DEF_DIAG_ENABLE" in line:
                diag_enabled = True
                diagnostic_evidence.append(evidence(root, path, line_no, line))
            if any(token in line for token in ("MasterReq", "SlaveResp", "ld_receive_message", "ld_send_message")):
                diagnostic_evidence.append(evidence(root, path, line_no, line))
            for match in sid_line.finditer(line):
                value = parse_int(match.group(1))
                if 0 <= value <= 0xFF:
                    sid_values.setdefault(value, []).append(
                        evidence(root, path, line_no, line)
                    )

    frames: Dict[Tuple[str, str], Dict[str, object]] = {}
    for signal in signals:
        key = (str(signal["frame"]), str(signal["direction"]))
        frame = frames.setdefault(
            key,
            {
                "name": signal["frame"],
                "direction": signal["direction"],
                "length": 8,
                "frame_id_or_pid": "review_required",
                "signals": [],
            },
        )
        frame["signals"].append(signal)
    for frame in frames.values():
        frame["signals"].sort(
            key=lambda item: (
                int(item["start_bit"]),
                str(item["signal"]),
            )
        )

    issues: List[Dict[str, str]] = []
    if not texts:
        issues.append({"severity": "error", "message": "no readable C sources found"})
    if not signals:
        issues.append(
            {
                "severity": "error",
                "message": "no generated LIN signal access macros found",
            }
        )
    if not nad_evidence:
        issues.append(
            {"severity": "error", "message": "no NAD handling evidence found"}
        )
    if diag_enabled and not diagnostic_evidence:
        issues.append(
            {
                "severity": "error",
                "message": "diagnostics enabled but request/response path was not found",
            }
        )
    if diag_enabled and not services:
        issues.append(
            {
                "severity": "warning",
                "message": "no proprietary DID table found; may be standard LIN diagnostics only",
            }
        )
    if not pair_tables:
        issues.append(
            {
                "severity": "warning",
                "message": "no NAD/address/PID pair table found; inspect computed mapping",
            }
        )
    for path in decode_errors:
        issues.append(
            {"severity": "warning", "message": "unreadable source skipped: " + path}
        )

    return {
        "schema_version": 1,
        "source_root": str(root),
        "source_file_count": len(texts),
        "addressing_contract": {
            "status": "review_required",
            "required_domains": [
                "diagnostic_initial_nad",
                "diagnostic_current_nad",
                "diagnostic_special_nad",
                "application_target_or_mask",
                "feedback_frame_id_or_pid",
            ],
            "candidate_pair_tables": pair_tables,
            "evidence": nad_evidence[:400],
        },
        "protocol_contract": {
            "status": "review_required",
            "frames": list(frames.values()),
            "diagnostics": {
                "enabled": diag_enabled,
                "request_frame": "review_required",
                "response_frame": "review_required",
                "request_nad_rule": "review_required",
                "response_nad_rule": "review_required",
                "candidate_sid_values": [
                    {
                        "sid": sid,
                        "sid_hex": "0x{0:02X}".format(sid),
                        "evidence": entries[:20],
                    }
                    for sid, entries in sorted(sid_values.items())
                ],
                "services": services,
                "evidence": diagnostic_evidence[:400],
            },
        },
        "review": {
            "status": "pending",
            "unresolved": [
                "Classify every address domain and confirm its valid range.",
                "Confirm control/status frame ID or PID and all signal fields.",
                "Confirm diagnostic request/response NAD rules and service framing.",
            ],
        },
        "issues": issues,
    }


def main(argv: Optional[Sequence[str]] = None) -> int:
    parser = argparse.ArgumentParser(
        description="Extract a LIN protocol contract draft from slave C code."
    )
    parser.add_argument("source_root")
    parser.add_argument("--output", required=True)
    args = parser.parse_args(argv)
    root = Path(args.source_root).resolve()
    output = Path(args.output).resolve()
    if not root.is_dir():
        print("source root does not exist: " + str(root), file=sys.stderr)
        return 2
    document = scan(root)
    output.parent.mkdir(parents=True, exist_ok=True)
    output.write_text(
        json.dumps(document, ensure_ascii=False, indent=2) + "\n",
        encoding="utf-8",
    )
    error_count = sum(
        1 for issue in document["issues"] if issue["severity"] == "error"
    )
    print(
        "LIN code scan: {0} sources, {1} frames, {2} DID services, "
        "{3} errors -> {4}".format(
            document["source_file_count"],
            len(document["protocol_contract"]["frames"]),
            len(document["protocol_contract"]["diagnostics"]["services"]),
            error_count,
            output,
        )
    )
    return 2 if error_count else 0


if __name__ == "__main__":
    sys.exit(main())
