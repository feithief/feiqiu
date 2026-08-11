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
        enum_minmax = re.search(
            r"q(?:Min|Max)\s*\([\s\S]{0,240},\s*"
            r"LinMaximumStatusFields\s*\)",
            text,
        )
        if enum_minmax:
            line = text.count("\n", 0, enum_minmax.start()) + 1
            errors.append(
                "{0}:{1}: qMin/qMax mixes int with anonymous enum; "
                "cast LinMaximumStatusFields to int".format(relative, line)
            )

    linlayout_runtime = find_source(source_texts, "linlayout")
    raw_status_semantic_guard = re.search(
        r"\(\s*field\.field\s*!=\s*ELinStatusRawValue\s*\)\s*&&\s*"
        r"\(\s*field\.normalValue\s*==\s*field\.errorValue\s*\)",
        linlayout_runtime or "",
    )
    if raw_status_semantic_guard is None:
        errors.append(
            "runtime layout validator must allow raw status fields without "
            "normal/error semantics"
        )

    architecture_files = {
        "linruntime.h": ("linruntime.h",),
        "lintransport.h": ("lintransport.h",),
        "debugsink.h": ("debugsink.h",),
        "debugsnapshot.h": ("debugsnapshot.h",),
        "debugstore.h": ("debugstore.h",),
        "ambientlinscheduler.h": ("ambientlinscheduler.h",),
        "ambientlinscheduler.cpp": (
            "ambientlinscheduler.cc",
            "ambientlinscheduler.cpp",
            "ambientlinscheduler.cpp.txt",
        ),
        "linbusworker.h": ("linbusworker.h",),
        "linbusworker.cpp": (
            "linbusworker.cc",
            "linbusworker.cpp",
            "linbusworker.cpp.txt",
        ),
        "ambientlincomm.h": ("ambientlincomm.h",),
        "ambientlincomm.cpp": (
            "ambientlincomm.cc",
            "ambientlincomm.cpp",
            "ambientlincomm.cpp.txt",
        ),
        "main.cpp": ("main.cc", "main.cpp", "main.cpp.txt"),
        "ARCHITECTURE.md": ("ARCHITECTURE.md", "ARCHITECTURE.md.txt"),
    }
    architecture_texts: Dict[str, str] = {}
    for name, candidates in architecture_files.items():
        failures: List[str] = []
        for candidate in candidates:
            path = project_root / candidate
            if not path.is_file():
                continue
            try:
                architecture_texts[name] = read_text(path)
                break
            except ValidationError as exc:
                failures.append(str(exc))
        if name not in architecture_texts:
            errors.append(
                "missing readable modular architecture file: {0}{1}".format(
                    name,
                    ("; " + "; ".join(failures)) if failures else "",
                )
            )

    require_marker(
        errors,
        architecture_texts.get("ambientlinscheduler.h"),
        "public LinRuntime",
        "runtime facade must implement the stable LinRuntime UI boundary",
    )
    require_marker(
        errors,
        architecture_texts.get("ambientlinscheduler.cpp"),
        "Qt::QueuedConnection",
        "GUI-to-worker commands must use queued value delivery",
    )
    require_marker(
        errors,
        architecture_texts.get("ambientlinscheduler.cpp"),
        "assertFacadeThread()",
        "runtime facade must assert GUI-thread ownership",
    )
    require_marker(
        errors,
        architecture_texts.get("linbusworker.cpp"),
        "assertWorkerThread()",
        "LIN worker must assert its single-thread execution context",
    )
    require_marker(
        errors,
        architecture_texts.get("linbusworker.cpp"),
        "DebugDiagnosticQueueDepth",
        "diagnostic queue depth must be visible in F12 debug data",
    )
    require_marker(
        errors,
        architecture_texts.get("linbusworker.h"),
        "LinTransport *comm",
        "worker must depend on the replaceable LinTransport port",
    )
    require_marker(
        errors,
        architecture_texts.get("ambientlincomm.cpp"),
        "assertOwnerThread()",
        "LIN file descriptor must enforce worker-thread ownership",
    )
    require_marker(
        errors,
        architecture_texts.get("main.cpp"),
        "AmbientLinCommFactory transportFactory",
        "main.cpp must remain the concrete transport composition root",
    )
    require_marker(
        errors,
        architecture_texts.get("debugstore.h"),
        "public DebugSink, public DebugSnapshotSource",
        "DebugStore must implement separate write and read observability ports",
    )

    debug_panel_header = project_root / "debugpanel.h"
    if not debug_panel_header.is_file():
        errors.append("missing debugpanel.h")
    else:
        debug_panel_text = read_text(debug_panel_header)
        if "DebugSnapshotSource" not in debug_panel_text:
            errors.append("F12 panel must depend on read-only DebugSnapshotSource")
    debug_panel_source = find_source(source_texts, "debugpanel")
    require_marker(
        errors,
        debug_panel_source,
        "debugSource->snapshot()",
        "F12 panel must read through DebugSnapshotSource",
    )
    require_marker(
        errors,
        debug_panel_source,
        "table->setRowCount(values.size())",
        "F12 panel must size itself from the snapshot instead of DebugVariableCount",
    )
    if debug_panel_source and (
        "debugStore" in debug_panel_source
        or "DebugVariableCount" in debug_panel_source
    ):
        errors.append(
            "debugpanel source still depends on concrete DebugStore/debug enum"
        )

    for ui_header in (
        "mainwindow.h",
        "bcmmasterframe.h",
        "productionverify.h",
        "slaveframeconfig.h",
    ):
        path = project_root / ui_header
        if not path.is_file():
            errors.append("missing UI boundary header: " + ui_header)
            continue
        text = read_text(path)
        if "AmbientLinScheduler" in text or "ambientlinscheduler.h" in text:
            errors.append(
                "{0}: UI feature depends on concrete scheduler".format(ui_header)
            )
        if "LinRuntime" not in text:
            errors.append(
                "{0}: UI feature is not injected through LinRuntime".format(
                    ui_header
                )
            )
        if ui_header == "mainwindow.h" and "DebugStore" in text:
            errors.append(
                "mainwindow.h: UI must use debug ports, not concrete DebugStore"
            )

    scheduler_text = (
        architecture_texts.get("ambientlinscheduler.h", "")
        + architecture_texts.get("ambientlinscheduler.cpp", "")
    )
    if "QMutex" in scheduler_text or "QMutexLocker" in scheduler_text:
        errors.append("runtime facade must not share mutable state through a mutex")

    mutex_candidates: List[tuple[str, str]] = []
    for path in project_root.glob("*.h"):
        mutex_candidates.append((path.name, read_text(path)))
    mutex_candidates.extend(source_texts.items())
    for relative, text in mutex_candidates:
        path = Path(relative)
        if path.stem.lower() == "debugstore":
            continue
        if "QMutex" in text or "QMutexLocker" in text:
            errors.append(
                "{0}: only DebugStore may own a cross-thread mutex".format(
                    relative
                )
            )

    report = load_report(project_root)
    generated = report.get("generated_profile", {})
    if not isinstance(generated, dict):
        errors.append("profile report has no generated_profile object")
        generated = {}

    # Complete-signal architecture is a Seed invariant, even when the current
    # profile happens to contain only the legacy RGB fields.  Future profiles
    # must not require another UI/runtime rewrite for an unknown source signal.
    lin_types_path = project_root / "lin_types.h"
    lin_types_text = read_text(lin_types_path) if lin_types_path.is_file() else ""
    layout_text = find_source(source_texts, "linlayout")
    signal_control_text = find_source(source_texts, "signalcontrolframe")
    require_marker(
        errors,
        lin_types_text,
        "ELinSignalRawValue",
        "LinLogicalSignal must preserve layout-specific raw control signals",
    )
    require_marker(
        errors,
        lin_types_text,
        "ELinColorModelGenericSignals",
        "Seed must support layouts without RGB/color-specific semantics",
    )
    require_marker(
        errors,
        lin_types_text,
        "rawSignalValues",
        "BCMSignal must store raw control values by exact source name",
    )
    require_marker(
        errors,
        layout_text,
        "case ELinSignalRawValue:",
        "LinLayout encode/default logic must handle raw control signals",
    )
    require_marker(
        errors,
        signal_control_text,
        "layout.publishedFrameCount",
        "all-signal page must enumerate every master-published frame",
    )
    require_marker(
        errors,
        signal_control_text,
        "controlledSignals.append(&signal)",
        "all-signal page must create one editor per generated layout entry",
    )
    if "signalcontrolframe.ui" not in project_text:
        errors.append("project does not register the all-signal Designer page")
    master_ui = project_root / "masterframe.ui"
    if (
        not master_ui.is_file()
        or "pushButtonAllSignals" not in read_text(master_ui)
    ):
        errors.append("master page has no visible all-signal navigation button")
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
    bulk_write = (
        diagnostics.get("bulk_write", [])
        if isinstance(diagnostics, dict)
        else []
    )
    write_completion_policy = (
        diagnostics.get("write_completion_policy", "positive_response")
        if isinstance(diagnostics, dict)
        else "positive_response"
    )
    if write_completion_policy not in (
        "positive_response",
        "send_only_then_delayed_0x22_readback",
    ):
        errors.append("unsupported diagnostic write-completion policy")
    if (
        write_completion_policy
        == "send_only_then_delayed_0x22_readback"
    ):
        for index, service in enumerate(services):
            if (
                isinstance(service, dict)
                and service.get("writable") is True
                and service.get("expect_positive_write_response") is not False
            ):
                errors.append(
                    "service {0} waits for 0x6E under send-only 0x2E policy"
                    .format(index)
                )
        if generated.get("bulk_write_readback_delay_ms", 0) <= 0:
            errors.append(
                "send-only 0x2E policy requires delayed 0x22 read-back"
            )
    signal_presets = generated.get("signal_presets", [])
    signal_preset_count = (
        len(signal_presets) if isinstance(signal_presets, list) else 0
    )

    if signal_preset_count > 30:
        linlayout = find_source(source_texts, "linlayout")
        require_marker(
            errors,
            linlayout,
            "(layout.signalPresetCount > 512)",
            "runtime layout validator still rejects paged preset tables above 30",
        )

    if node_count > 0 and diagnostic_model != "none":
        mainwindow = find_source(source_texts, "mainwindow")
        slavebutton = find_source(source_texts, "slavebutton")
        slaveconfig = find_source(source_texts, "slaveframeconfig")
        worker = find_source(source_texts, "linbusworker")
        runtime_header = architecture_texts.get("linruntime.h")
        slave_ui_path = project_root / "slavenodeframe.ui"
        slave_ui = read_text(slave_ui_path) if slave_ui_path.is_file() else None
        if slave_ui is None:
            errors.append("diagnostic page UI is missing: slavenodeframe.ui")
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
            mainwindow,
            "SIGNAL(configurationReady(int))",
            "custom-DID page must wait for a configuration-ready signal",
        )
        require_marker(
            errors,
            mainwindow,
            "void MainWindow::showSlaveConfig",
            "diagnostic page reveal must be separated from its read request",
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
        require_marker(
            errors,
            runtime_header,
            "nodeResponseObserved(quint8 node)",
            "the diagnostic watchdog must observe every valid node response",
        )
        require_marker(
            errors,
            slaveconfig,
            "feedbackWatchdog->setInterval(5000)",
            "diagnostic page must close after 5000 ms without node feedback",
        )
        require_marker(
            errors,
            slaveconfig,
            "configurationRetryTimer->setInterval(250)",
            "failed initial configuration reads must retry while the page stays hidden",
        )
        require_marker(
            errors,
            slaveconfig,
            "emit configurationReady(currentNode)",
            "diagnostic page must be released only after configuration is ready",
        )
        for editor_name in (
            "spinBoxSA",
            "spinBoxPlatform",
            "spinBoxIntensity",
            "doubleSpinBoxRX",
            "doubleSpinBoxRY",
            "doubleSpinBoxRL",
            "doubleSpinBoxGX",
            "doubleSpinBoxGY",
            "doubleSpinBoxGL",
            "doubleSpinBoxBX",
            "doubleSpinBoxBY",
            "doubleSpinBoxBL",
        ):
            require_marker(
                errors,
                slaveconfig,
                "ui->{0}->interpretText();".format(editor_name),
                "diagnostic write must commit editor {0} before snapshotting".format(
                    editor_name
                ),
            )
        require_marker(
            errors,
            linlayout_runtime,
            "qRound64(scaledLuminance)",
            "fixed-point diagnostic luminance must be rounded before encoding",
        )
        require_marker(
            errors,
            slaveconfig,
            "status.rawFieldValues[index]",
            "status page must show every layout field's raw signal value",
        )
        require_marker(
            errors,
            slaveconfig,
            "status.rawFieldCount",
            "status page must size itself from the active feedback layout",
        )
        require_marker(
            errors,
            slaveconfig,
            "ui->statusTable->setRowCount(rowCount)",
            "status page must create one row for every valid feedback signal",
        )
        require_marker(
            errors,
            slaveconfig,
            "status.rawFrame.toHex",
            "status page must retain a raw-frame fallback",
        )
        require_marker(
            errors,
            slaveconfig,
            "linRuntime->lockNode",
            "Lock State must expose the manual Lock command",
        )
        require_marker(
            errors,
            slaveconfig,
            "linRuntime->unlockNode",
            "Lock State must expose the manual Unlock command",
        )
        require_marker(
            errors,
            slaveconfig,
            "pushButtonLockAction",
            "manual Lock must use a separate action button below Lock State",
        )
        require_marker(
            errors,
            slaveconfig,
            "pushButtonUnlockAction",
            "manual Unlock must use a separate action button below Lock State",
        )
        require_marker(
            errors,
            slave_ui,
            'name="pushButtonLockAction"',
            "diagnostic UI must contain a separate Lock action button",
        )
        require_marker(
            errors,
            slave_ui,
            'name="pushButtonUnlockAction"',
            "diagnostic UI must contain a separate Unlock action button",
        )
        require_marker(
            errors,
            slave_ui,
            "<string>Locked</string>",
            "Lock State must retain the read-only Locked indicator",
        )
        require_marker(
            errors,
            slave_ui,
            "<string>Unlocked</string>",
            "Lock State must retain the read-only Unlocked indicator",
        )
        if slaveconfig and (
            "connect(ui->pushButtonLock, SIGNAL(clicked())" in slaveconfig
            or "connect(ui->pushButtonUnlock, SIGNAL(clicked())" in slaveconfig
        ):
            errors.append(
                "Lock State indicators must not be connected as action buttons"
            )
        if slaveconfig and any(
            marker in slaveconfig for marker in (
                'setText("Normal")',
                'setText("Error")',
                'setText("Unknown")',
            )
        ):
            errors.append(
                "status page still translates raw values to Normal/Error/Unknown"
            )
        if mainwindow and (
            "unlockfrom" in mainwindow.lower()
            or "unlockFeature" in mainwindow
        ):
            errors.append("standalone Unlock page/button must not exist")
        if "unlockfrom" in project_text.lower():
            errors.append("qmake project still includes standalone Unlock sources")
        if worker:
            require_marker(
                errors,
                worker,
                "seed + 0x00000C04u",
                "manual Unlock must use uint32 little-endian seed + 0x0C04",
            )
            require_marker(
                errors,
                worker,
                "value.append(static_cast<char>(0x82))",
                "manual Lock must write DID 0002 payload 82 00",
            )
            require_marker(
                errors,
                worker,
                "QTimer::singleShot(1000, this, SLOT(processTaskStep()))",
                "Lock/Unlock must wait 1000 ms before DID 0002 read-back",
            )
            require_marker(
                errors,
                worker,
                "readServiceValue(activeInitialNad, *service, &readBack)",
                "Lock/Unlock must verify state with a fresh 0x22 DID 0002 read",
            )
            require_marker(
                errors,
                worker,
                "(flags & 0x0080u) != 0",
                "Lock State must come from DID 0002 IsLocked bit7",
            )
            write_service = re.search(
                r"bool\s+LinBusWorker::writeServiceValue\s*\("
                r"[\s\S]*?(?=\nSlaveConfigInfo\s+LinBusWorker::)",
                worker,
            )
            if write_service and "unlockSecurity(" in write_service.group(0):
                errors.append(
                    "ordinary 0x2E write still triggers automatic SecurityAccess"
                )
            if write_service and (
                "receiveDiagnosticFrame(" in write_service.group(0)
                or "0x6E" in write_service.group(0)
            ):
                errors.append("0x2E write still waits for a 0x6E response")
            require_marker(
                errors,
                write_service.group(0) if write_service else None,
                "0x2E is send-only",
                "0x2E send-only policy is missing from writeServiceValue",
            )
        if isinstance(diagnostics, dict):
            if diagnostics.get(
                "diagnostic_page_response_watchdog_ms"
            ) != 5000:
                errors.append("profile watchdog must be 5000 ms")
            if diagnostics.get(
                "status_display"
            ) != "short_signal_name_and_raw_hex":
                errors.append("profile status display must be raw hexadecimal")
            security = diagnostics.get("security")
            if isinstance(security, dict) and security.get("enabled") is True:
                if (
                    security.get("key_addend") != 0x0C04
                    or security.get("key_length") != 4
                    or security.get("manual_trigger_only") is not True
                    or security.get("lock_service_id") != 0x0002
                    or security.get("lock_payload") != [0x82, 0x00]
                ):
                    errors.append(
                        "profile manual SecurityAccess/Lock contract is invalid"
                    )
                if (
                    security.get("lock_state_readback_delay_ms") != 1000
                    or security.get("lock_state_readback_service") != 0x22
                ):
                    errors.append(
                        "profile Lock/Unlock must read DID0002 with 0x22 after 1000 ms"
                    )
        if slaveconfig and re.search(
            r"diagnosticModel\s*!=\s*ELinDiagnosticModelCustomDid[\s\S]{0,180}"
            r"\bhide\s*\(\s*\)\s*;\s*return\s*;",
            slaveconfig,
        ):
            errors.append(
                "diagnostic page still hides itself when proprietary DID services are absent"
            )

        if isinstance(bulk_write, list) and bulk_write:
            readback_delay = generated.get("bulk_write_readback_delay_ms")
            if (
                not isinstance(readback_delay, int)
                or isinstance(readback_delay, bool)
                or readback_delay < 0
                or readback_delay > 10000
            ):
                errors.append(
                    "bulk-write read-back delay is missing or outside 0..10000 ms"
                )
            require_marker(
                errors,
                worker,
                "EWriteTaskPhaseWaitForFlash",
                "bulk writes must have a separate flash-wait phase",
            )
            require_marker(
                errors,
                worker,
                "QTimer::singleShot(linLayout->bulkWriteReadBackDelayMs",
                "bulk flash wait must use the configured non-blocking timer",
            )
            require_marker(
                errors,
                worker,
                "Read-back %1 failed after flash wait",
                "bulk writes must perform one unified read-back pass after flash wait",
            )

    if generated.get("startup_output_policy") != "visible_on_power_up":
        errors.append("profile must publish a visible LED-on command at startup")
    published_frames = generated.get("published_frames", [])
    if isinstance(published_frames, list):
        for frame in published_frames:
            if not isinstance(frame, dict):
                continue
            seen_source_names = set()
            bindings = frame.get("bindings", [])
            if not isinstance(bindings, list):
                errors.append("published frame bindings must be a list")
                continue
            for binding in bindings:
                if not isinstance(binding, dict):
                    errors.append("published frame contains an invalid binding")
                    continue
                source_names = binding.get("source_signals")
                if not isinstance(source_names, list) or not source_names:
                    errors.append(
                        "published binding has no source_signals: "
                        + str(binding.get("name"))
                    )
                    continue
                for source_name in source_names:
                    if not isinstance(source_name, str) or not source_name:
                        errors.append("published binding has an invalid source name")
                    elif source_name in seen_source_names:
                        errors.append(
                            "published frame duplicates source signal: "
                            + source_name
                        )
                    else:
                        seen_source_names.add(source_name)
    primary_index = generated.get("primary_control_frame_index", -1)
    if (
        isinstance(published_frames, list)
        and isinstance(primary_index, int)
        and 0 <= primary_index < len(published_frames)
    ):
        primary = published_frames[primary_index]
        defaults = {
            item.get("semantic"): item.get("default")
            for item in primary.get("bindings", [])
            if isinstance(item, dict)
        }
        color_model = (
            models.get("color") if isinstance(models, dict) else None
        )
        if color_model != "generic_signals":
            if defaults.get("target_mask", 0) == 0:
                errors.append("startup target mask must select at least one LED")
            if defaults.get("intensity", 0) == 0:
                errors.append("startup intensity must be non-zero")
        if "led_enable" in defaults and defaults.get("led_enable") != 1:
            errors.append("startup LED-enable signal must be one")
        if color_model in ("direct_rgb_only", "selectable"):
            if not any(defaults.get(name, 0) for name in (
                "red_or_predefined", "green", "blue"
            )):
                errors.append("startup direct RGB values must be visible")
        required_nonzero = generated.get(
            "startup_required_nonzero_signals", []
        )
        all_defaults = {
            source_name: item.get("default", 0)
            for frame in published_frames
            if isinstance(frame, dict)
            for item in frame.get("bindings", [])
            if isinstance(item, dict)
            for source_name in item.get("source_signals", [])
        }
        for signal_name in required_nonzero:
            if all_defaults.get(signal_name, 0) == 0:
                errors.append(
                    "startup-required signal is zero or absent: " + signal_name
                )
            if worker:
                write_service = re.search(
                    r"bool\s+LinBusWorker::writeServiceValue\s*\("
                    r"[\s\S]*?(?=\nSlaveConfigInfo\s+LinBusWorker::)",
                    worker,
                )
                if (
                    write_service
                    and "readServiceValue(" in write_service.group(0)
                ):
                    errors.append(
                        "writeServiceValue still performs speculative per-DID read-back"
                    )
            debugstore = find_source(source_texts, "debugstore")
            require_marker(
                errors,
                debugstore,
                '"Diagnostic.LastError"',
                "dedicated diagnostic error must not be overwritten by normal I/O",
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
