#!/usr/bin/env python3
"""Fast static acceptance checks for a generated Qt LIN host."""

from __future__ import annotations

import argparse
import json
import re
import sys
from pathlib import Path
from typing import Dict, Iterable, List, Optional, Sequence


class ValidationError(Exception):
    """One or more generated-host acceptance checks failed."""


def read_text(path: Path) -> str:
    try:
        return path.read_text(encoding="utf-8-sig")
    except UnicodeDecodeError as exc:
        raise ValidationError(
            "{0} is not readable UTF-8 text (possibly DLP protected): {1}".format(
                path, exc
            )
        )


def parse_qmake_list(project_text: str, variable: str) -> List[str]:
    logical_lines: List[str] = []
    pending = ""
    for raw_line in project_text.splitlines():
        line = raw_line.split("#", 1)[0].rstrip()
        if not line:
            continue
        pending = (pending + " " + line).strip()
        if pending.endswith("\\"):
            pending = pending[:-1].rstrip()
            continue
        logical_lines.append(pending)
        pending = ""
    if pending:
        logical_lines.append(pending)

    result: List[str] = []
    for line in logical_lines:
        match = re.match(
            r"^\s*{0}\s*\+=\s*(.*?)\s*$".format(re.escape(variable)), line
        )
        if match:
            result.extend(match.group(1).split())
    return result


def check_cpp_lexing(path: Path, text: str) -> List[str]:
    errors: List[str] = []
    state = "code"
    escaped = False
    line = 1
    start_line = 1
    index = 0
    while index < len(text):
        char = text[index]
        following = text[index + 1] if index + 1 < len(text) else ""
        if char == "\n":
            if state in {"string", "character"} and not escaped:
                errors.append(
                    "{0}:{1}: unterminated {2} literal".format(
                        path.name, start_line, state
                    )
                )
                state = "code"
            line += 1
            escaped = False
            if state == "line_comment":
                state = "code"
            index += 1
            continue

        if state == "code":
            if char == "/" and following == "/":
                state = "line_comment"
                index += 2
                continue
            if char == "/" and following == "*":
                state = "block_comment"
                start_line = line
                index += 2
                continue
            if char == '"':
                state = "string"
                start_line = line
                escaped = False
            elif char == "'":
                state = "character"
                start_line = line
                escaped = False
        elif state == "block_comment":
            if char == "*" and following == "/":
                state = "code"
                index += 2
                continue
        elif state in {"string", "character"}:
            closing = '"' if state == "string" else "'"
            if char == closing and not escaped:
                state = "code"
            if char == "\\" and not escaped:
                escaped = True
            else:
                escaped = False
        index += 1

    if state in {"string", "character", "block_comment"}:
        errors.append(
            "{0}:{1}: unterminated {2}".format(path.name, start_line, state)
        )
    return errors


def find_source(sources: Dict[str, str], stem: str) -> Optional[str]:
    for relative, text in sources.items():
        if Path(relative).stem.lower() == stem.lower():
            return text
    return None


def require_marker(
    errors: List[str], text: Optional[str], marker: str, message: str
) -> None:
    if text is None or marker not in text:
        errors.append(message)


def require_any_marker(
    errors: List[str],
    text: Optional[str],
    markers: Sequence[str],
    message: str,
) -> None:
    if text is None or not any(marker in text for marker in markers):
        errors.append(message)


def load_report(project_root: Path) -> Dict[str, object]:
    candidates = [
        project_root / "generated" / "linprofile_report.json",
        project_root / "generated" / "linprofile_report.json.txt",
    ]
    failures: List[str] = []
    for candidate in candidates:
        if not candidate.is_file():
            continue
        try:
            return json.loads(candidate.read_text(encoding="utf-8-sig"))
        except (UnicodeDecodeError, json.JSONDecodeError) as exc:
            failures.append("{0}: {1}".format(candidate.name, exc))
    raise ValidationError(
        "no readable generated profile report; " + "; ".join(failures)
    )


def validate(project_root: Path) -> None:
    project_files = sorted(project_root.glob("*.pro"))
    if len(project_files) != 1:
        raise ValidationError("project root must contain exactly one .pro file")

    project_text = read_text(project_files[0])
    source_names = parse_qmake_list(project_text, "SOURCES")
    if not source_names:
        raise ValidationError("the .pro file contains no SOURCES")

    errors: List[str] = []
    source_texts: Dict[str, str] = {}
    for relative in source_names:
        path = project_root / relative
        if not path.is_file():
            errors.append("missing compiler source: " + relative)
            continue
        try:
            text = read_text(path)
        except ValidationError as exc:
            errors.append(str(exc))
            continue
        source_texts[relative] = text
        errors.extend(check_cpp_lexing(path, text))
        collision = re.search(
            r"\b(?:auto|bool|char|double|float|int|long|short|QString|"
            r"QByteArray|BCMSignal)\s*(?:const\s+)?[*&]?\s*signals\b",
            text,
        )
        if collision:
            line = text.count("\n", 0, collision.start()) + 1
            errors.append(
                "{0}:{1}: identifier 'signals' collides with Qt's macro".format(
                    relative, line
                )
            )

    report = load_report(project_root)
    generated = report.get("generated_profile", {})
    if not isinstance(generated, dict):
        errors.append("profile report has no generated_profile object")
        generated = {}
    nodes = generated.get("nodes", [])
    models = generated.get("models", {})
    diagnostics = generated.get("diagnostics", {})
    node_count = len(nodes) if isinstance(nodes, list) else 0
    diagnostic_model = (
        models.get("diagnostic", "unknown")
        if isinstance(models, dict)
        else "unknown"
    )
    services = (
        diagnostics.get("services", [])
        if isinstance(diagnostics, dict)
        else []
    )
    service_count = len(services) if isinstance(services, list) else 0

    if node_count > 0 and diagnostic_model != "none":
        mainwindow = find_source(source_texts, "mainwindow")
        slavebutton = find_source(source_texts, "slavebutton")
        slaveconfig = find_source(source_texts, "slaveframeconfig")
        require_marker(
            errors,
            mainwindow,
            "connect(button, SIGNAL(slaveClicked(int))",
            "diagnostic navigation missing: every node button must connect its click",
        )
        require_marker(
            errors,
            mainwindow,
            "slaveFrame = new SlaveFrameConfig",
            "diagnostic navigation missing: node page must always be created",
        )
        require_any_marker(
            errors,
            slavebutton,
            ("setEnabled(feedbackAvailable);", "setEnabled(online);"),
            "diagnostic feedback gate missing: node button must follow validated feedback",
        )
        require_any_marker(
            errors,
            slavebutton,
            ("if (!feedbackAvailable)", "if (!online)"),
            "diagnostic feedback gate missing: click handler needs a defensive guard",
        )
        require_any_marker(
            errors,
            mainwindow,
            ("!button->hasValidFeedback()", "!button->isOnLine()"),
            "diagnostic feedback gate missing: MainWindow must recheck before navigation",
        )
        require_marker(
            errors,
            slaveconfig,
            "configurationAvailable",
            "diagnostic mode split missing: status-only/custom-DID state is required",
        )
        require_marker(
            errors,
            slaveconfig,
            "if (!configurationAvailable)",
            "status-only guard missing: do not send guessed proprietary DID requests",
        )
        if slaveconfig and re.search(
            r"diagnosticModel\s*!=\s*ELinDiagnosticModelCustomDid[\s\S]{0,180}"
            r"\bhide\s*\(\s*\)\s*;\s*return\s*;",
            slaveconfig,
        ):
            errors.append(
                "diagnostic page still hides itself when proprietary DID services are absent"
            )

    if errors:
        raise ValidationError("\n- ".join(["static acceptance failed"] + errors))

    print(
        "static acceptance OK: {0} sources, {1} nodes, diagnostics={2}, "
        "services={3}".format(
            len(source_texts), node_count, diagnostic_model, service_count
        )
    )


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(
        description="Validate a generated Qt LIN host without compiling Qt."
    )
    parser.add_argument("project_root")
    return parser


def main(argv: Optional[Sequence[str]] = None) -> int:
    args = build_parser().parse_args(argv)
    try:
        validate(Path(args.project_root).resolve())
        return 0
    except (OSError, ValidationError) as exc:
        print(str(exc), file=sys.stderr)
        return 2


if __name__ == "__main__":
    sys.exit(main())
