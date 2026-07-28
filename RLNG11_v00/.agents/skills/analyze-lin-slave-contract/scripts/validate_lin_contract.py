#!/usr/bin/env python3
"""Validate the reviewed code-only LIN contract before host generation."""

from __future__ import annotations

import argparse
import json
import sys
from pathlib import Path
from typing import Dict, List, Optional, Sequence


class ContractError(Exception):
    pass


def load_json(path: Path) -> Dict[str, object]:
    value = json.loads(path.read_text(encoding="utf-8-sig"))
    if not isinstance(value, dict):
        raise ContractError("contract root must be an object")
    return value


def validate_evidence(
    errors: List[str], source_root: Path, evidence: object, label: str
) -> None:
    if not isinstance(evidence, list) or not evidence:
        errors.append(label + " has no source evidence")
        return
    for index, item in enumerate(evidence):
        if not isinstance(item, dict):
            errors.append("{0}[{1}] is not an object".format(label, index))
            continue
        relative = item.get("file")
        line = item.get("line")
        contains = item.get("contains")
        if not isinstance(relative, str) or not isinstance(line, int):
            errors.append("{0}[{1}] has invalid file/line".format(label, index))
            continue
        path = source_root / relative
        if not path.is_file():
            errors.append("{0}[{1}] source missing: {2}".format(label, index, relative))
            continue
        text = path.read_text(encoding="latin-1")
        lines = text.splitlines()
        if line < 1 or line > len(lines):
            errors.append("{0}[{1}] line is outside source".format(label, index))
        elif isinstance(contains, str) and contains:
            marker = contains.strip()[:32]
            if marker and marker not in lines[line - 1]:
                errors.append(
                    "{0}[{1}] evidence text no longer matches source".format(
                        label, index
                    )
                )


def validate(contract: Dict[str, object], source_root: Path) -> None:
    errors: List[str] = []
    if contract.get("schema_version") != 1:
        errors.append("schema_version must be 1")
    addressing = contract.get("addressing_contract")
    protocol = contract.get("protocol_contract")
    review = contract.get("review")
    if not isinstance(addressing, dict):
        errors.append("addressing_contract is missing")
        addressing = {}
    if not isinstance(protocol, dict):
        errors.append("protocol_contract is missing")
        protocol = {}
    if not isinstance(review, dict):
        errors.append("review is missing")
        review = {}
    if review.get("status") != "confirmed":
        errors.append("review.status must be confirmed")
    unresolved = review.get("unresolved")
    if unresolved not in ([], None):
        errors.append("review.unresolved must be empty")
    if addressing.get("status") != "confirmed":
        errors.append("addressing_contract.status must be confirmed")
    if protocol.get("status") != "confirmed":
        errors.append("protocol_contract.status must be confirmed")

    domains = addressing.get("domains")
    if not isinstance(domains, list) or not domains:
        errors.append("addressing_contract.domains must contain reviewed domains")
    else:
        names = {
            item.get("name")
            for item in domains
            if isinstance(item, dict)
        }
        required = {"diagnostic_current_nad", "application_target_or_mask"}
        missing = required - names
        if missing:
            errors.append("address domains missing: " + ", ".join(sorted(missing)))
        for index, item in enumerate(domains):
            if isinstance(item, dict):
                validate_evidence(
                    errors,
                    source_root,
                    item.get("evidence"),
                    "addressing_contract.domains[{0}].evidence".format(index),
                )

    frames = protocol.get("frames")
    if not isinstance(frames, list) or not frames:
        errors.append("protocol_contract.frames must not be empty")
    else:
        for frame_index, frame in enumerate(frames):
            if not isinstance(frame, dict):
                errors.append("frame {0} is not an object".format(frame_index))
                continue
            length = frame.get("length")
            if not isinstance(length, int) or length <= 0:
                errors.append("frame {0} has invalid length".format(frame_index))
                continue
            occupied = set()
            for signal_index, signal in enumerate(frame.get("signals", [])):
                if not isinstance(signal, dict):
                    continue
                start = signal.get("start_bit")
                width = signal.get("bit_length")
                if not isinstance(start, int) or not isinstance(width, int) or width <= 0:
                    errors.append(
                        "frame {0} signal {1} has unresolved bit layout".format(
                            frame_index, signal_index
                        )
                    )
                    continue
                bits = set(range(start, start + width))
                if max(bits) >= length * 8:
                    errors.append(
                        "frame {0} signal {1} exceeds frame length".format(
                            frame_index, signal_index
                        )
                    )
                if occupied & bits:
                    errors.append(
                        "frame {0} signal {1} overlaps another signal".format(
                            frame_index, signal_index
                        )
                    )
                occupied |= bits
                validate_evidence(
                    errors,
                    source_root,
                    signal.get("evidence"),
                    "frame[{0}].signal[{1}].evidence".format(
                        frame_index, signal_index
                    ),
                )

    diagnostics = protocol.get("diagnostics")
    if isinstance(diagnostics, dict) and diagnostics.get("enabled") is True:
        for key in (
            "request_frame",
            "response_frame",
            "request_nad_rule",
            "response_nad_rule",
        ):
            value = diagnostics.get(key)
            if not isinstance(value, str) or not value or value == "review_required":
                errors.append("diagnostics.{0} is unresolved".format(key))
        validate_evidence(
            errors,
            source_root,
            diagnostics.get("evidence"),
            "protocol_contract.diagnostics.evidence",
        )

    if errors:
        raise ContractError("\n- ".join(["contract validation failed"] + errors))


def main(argv: Optional[Sequence[str]] = None) -> int:
    parser = argparse.ArgumentParser(
        description="Validate a reviewed code-only LIN contract."
    )
    parser.add_argument("contract")
    parser.add_argument("--source-root", required=True)
    args = parser.parse_args(argv)
    try:
        validate(
            load_json(Path(args.contract).resolve()),
            Path(args.source_root).resolve(),
        )
        print("LIN contract validation OK")
        return 0
    except (OSError, json.JSONDecodeError, ContractError) as exc:
        print(str(exc), file=sys.stderr)
        return 2


if __name__ == "__main__":
    sys.exit(main())
