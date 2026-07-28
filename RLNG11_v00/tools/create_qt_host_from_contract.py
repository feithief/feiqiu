#!/usr/bin/env python3
"""Create a mother-Seed Qt LIN host from a confirmed slave-C contract."""

from __future__ import annotations

import argparse
import hashlib
import importlib.util
import json
import os
import re
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path
from typing import Any, Dict, List, Optional, Sequence


class CreateError(Exception):
    pass


PROJECT_NAME_RE = re.compile(r"^[A-Za-z][A-Za-z0-9_]*$")


def load_json(path: Path) -> Dict[str, Any]:
    failures: List[str] = []
    for candidate in (path, Path(str(path) + ".txt")):
        if not candidate.is_file():
            continue
        try:
            value = json.loads(candidate.read_text(encoding="utf-8-sig"))
            if not isinstance(value, dict):
                raise CreateError(str(candidate) + " must contain a JSON object")
            return value
        except (UnicodeDecodeError, json.JSONDecodeError) as exc:
            failures.append("{0}: {1}".format(candidate.name, exc))
    raise CreateError("no readable JSON input: " + "; ".join(failures))


def sha256(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def run_checked(command: List[str], cwd: Path) -> None:
    completed = subprocess.run(command, cwd=str(cwd), check=False)
    if completed.returncode != 0:
        raise CreateError(
            "command failed ({0}): {1}".format(
                completed.returncode, " ".join(command)
            )
        )


def import_emitter(seed_root: Path) -> Any:
    path = seed_root / "tools" / "ldf_profile_gen.py"
    spec = importlib.util.spec_from_file_location("lin_profile_emitter", path)
    if spec is None or spec.loader is None:
        raise CreateError("cannot load profile emitter: " + str(path))
    module = importlib.util.module_from_spec(spec)
    sys.modules[spec.name] = module
    spec.loader.exec_module(module)
    return module


def should_skip(name: str) -> bool:
    lower = name.lower()
    return (
        name in {".git", ".agents", "tools", "profiles", "LDF"}
        or lower == "makefile"
        or lower.endswith(".pro.user")
        or lower.endswith(".pyc")
        or name == "__pycache__"
        or lower.startswith("build-")
        or lower.endswith(".tmp")
    )


def copy_seed_project(source: Path, target: Path) -> None:
    for entry in source.iterdir():
        if should_skip(entry.name):
            continue
        destination = target / entry.name
        if entry.is_dir():
            shutil.copytree(
                entry,
                destination,
                ignore=shutil.ignore_patterns("__pycache__", "*.pyc", "*.tmp"),
            )
        else:
            shutil.copy2(entry, destination)


def require_profile(profile: Dict[str, Any], emitter: Any) -> None:
    required = (
        "source",
        "layout_name",
        "device_name",
        "baud_rate",
        "serial_io_timeout_ms",
        "models",
        "intensity_maximum",
        "dimming_maximum",
        "predefined_colors",
        "published_frames",
        "primary_control_frame_index",
        "status_layouts",
        "nodes",
        "diagnostics",
        "schedule",
        "diagnostic_request_to_response_delay_ms",
        "diagnostic_inter_frame_delay_ms",
        "response_delay_ms",
        "post_write_settle_ms",
        "maximum_diagnostic_queue_depth",
    )
    missing = [name for name in required if name not in profile]
    if missing:
        raise CreateError("host_profile fields missing: " + ", ".join(missing))
    if not isinstance(profile["published_frames"], list) or not profile["published_frames"]:
        raise CreateError("host_profile.published_frames must not be empty")
    if not isinstance(profile["nodes"], list) or not profile["nodes"]:
        raise CreateError("host_profile.nodes must not be empty")

    frame = profile["published_frames"][profile["primary_control_frame_index"]]
    semantics = {item.get("semantic") for item in frame.get("bindings", [])}
    expected = {"target_mask", "intensity", "red_or_predefined", "green", "blue"}
    if not expected.issubset(semantics):
        raise CreateError(
            "primary control frame lacks: " + ", ".join(sorted(expected - semantics))
        )
    for item in frame["bindings"]:
        semantic = item.get("semantic")
        if semantic not in emitter.LOGICAL_SIGNAL_ENUMS:
            raise CreateError("unsupported logical signal: " + str(semantic))
        if item.get("enum") != emitter.LOGICAL_SIGNAL_ENUMS[semantic]:
            raise CreateError("logical-signal enum mismatch: " + str(semantic))

    nads = [item.get("nad") for item in profile["nodes"]]
    masks = [item.get("control_mask") for item in profile["nodes"]]
    status_ids = [item.get("status_frame_id") for item in profile["nodes"]]
    if len(set(nads)) != len(nads):
        raise CreateError("node NAD values must be unique")
    if len(set(masks)) != len(masks):
        raise CreateError("node target masks must be unique")
    if len(set(status_ids)) != len(status_ids):
        raise CreateError("node feedback frame IDs must be unique")


def attach_presets(
    profile: Dict[str, Any],
    preset_doc: Optional[Dict[str, Any]],
) -> None:
    profile["signal_preset_group_name"] = ""
    profile["signal_presets"] = []
    if preset_doc is None:
        return
    root = preset_doc.get("signal_presets")
    if not isinstance(root, dict):
        raise CreateError("preset file has no signal_presets object")
    group_name = root.get("group_name")
    items = root.get("items")
    if not isinstance(group_name, str) or not group_name:
        raise CreateError("signal_presets.group_name is empty")
    if not isinstance(items, list) or len(items) > 512:
        raise CreateError("signal preset count must be 0..512")

    frame = profile["published_frames"][profile["primary_control_frame_index"]]
    binding_by_name = {item["name"]: item for item in frame["bindings"]}
    seen = set()
    output = []
    for index, item in enumerate(items):
        if not isinstance(item, dict):
            raise CreateError("signal preset item is not an object")
        name = item.get("name")
        values = item.get("values")
        if not isinstance(name, str) or not name or name in seen:
            raise CreateError("signal preset name is empty or duplicated")
        if not isinstance(values, dict) or not values:
            raise CreateError("signal preset values are empty at index " + str(index))
        seen.add(name)
        assignments = []
        for signal_name, value in values.items():
            binding = binding_by_name.get(signal_name)
            if binding is None:
                raise CreateError(
                    "preset signal is outside the primary control frame: " + signal_name
                )
            maximum = (1 << binding["bit_length"]) - 1
            if (
                not isinstance(value, int)
                or isinstance(value, bool)
                or value < 0
                or value > maximum
            ):
                raise CreateError("preset value is outside signal width: " + signal_name)
            assignments.append(
                {
                    "signal": signal_name,
                    "start_bit": binding["start_bit"],
                    "bit_length": binding["bit_length"],
                    "value": value,
                }
            )
        output.append({"name": name, "assignments": assignments})
    profile["signal_preset_group_name"] = group_name
    profile["signal_presets"] = output


def compiler_safe_sources(project_root: Path, project_file: Path) -> None:
    """Compile readable .cc mirrors so DLP-wrapped .cpp files cannot enter qmake."""
    project_text = project_file.read_text(encoding="utf-8-sig")
    sources = re.findall(
        r"(?m)^\s*([A-Za-z0-9_./-]+\.cpp)\s*\\?\s*$", project_text
    )
    replacements: Dict[str, str] = {}
    for relative in sources:
        source = project_root / relative
        if not source.is_file():
            continue
        sibling = source.with_suffix(".cc")
        try:
            content = source.read_text(encoding="utf-8-sig")
            sibling.write_text(content, encoding="utf-8", newline="\n")
        except UnicodeDecodeError:
            if not sibling.is_file():
                raise CreateError("DLP-protected compiler source has no .cc mirror: " + relative)
            sibling.read_text(encoding="utf-8-sig")
        replacements[relative] = str(Path(relative).with_suffix(".cc")).replace("\\", "/")
    for old, new in replacements.items():
        project_text = re.sub(
            r"(?m)(^\s*)" + re.escape(old) + r"(\s*\\?\s*$)",
            lambda match: match.group(1) + new + match.group(2),
            project_text,
        )
    project_file.write_text(project_text, encoding="utf-8", newline="\n")


def build_report(
    contract: Dict[str, Any],
    contract_path: Path,
    profile: Dict[str, Any],
) -> Dict[str, Any]:
    return {
        "schema_version": 1,
        "source_code_contract": {
            "name": contract_path.name,
            "sha256": sha256(contract_path),
            "source_root": contract.get("source_root"),
            "review": contract.get("review"),
        },
        "generated_profile": profile,
        "integration": {
            "ready_for_linlayout_types": True,
            "ambientdevice_modified_by_generator": False,
            "input_mode": "reviewed_slave_c_code_contract",
            "qt_compilation": "skipped_by_policy",
        },
    }


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(
        description="Create an independent Qt LIN host from reviewed slave C code."
    )
    parser.add_argument("--project-name", required=True)
    parser.add_argument("--contract", required=True)
    parser.add_argument("--presets")
    parser.add_argument("--destination", required=True)
    parser.add_argument("--seed-root")
    return parser


def main(argv: Optional[Sequence[str]] = None) -> int:
    args = build_parser().parse_args(argv)
    stage: Optional[Path] = None
    try:
        if not PROJECT_NAME_RE.fullmatch(args.project_name):
            raise CreateError(
                "project name must start with a letter and contain only letters, digits, and underscores"
            )
        seed_root = (
            Path(args.seed_root).resolve()
            if args.seed_root
            else Path(__file__).resolve().parents[1]
        )
        seed_project = seed_root / "AmbientDevice"
        if not (seed_project / "AmbientDevice.pro").is_file():
            raise CreateError("mother Seed project is missing")
        contract_path = Path(args.contract).resolve()
        contract = load_json(contract_path)
        preset_path = Path(args.presets).resolve() if args.presets else None
        presets = load_json(preset_path) if preset_path else None
        source_root_value = contract.get("source_root")
        if not isinstance(source_root_value, str) or not Path(source_root_value).is_dir():
            raise CreateError("contract source_root is missing")
        if (
            contract.get("review", {}).get("status") != "confirmed"
            or contract.get("review", {}).get("unresolved") not in ([], None)
        ):
            raise CreateError("contract review is not confirmed")

        contract_validator = (
            seed_root
            / ".agents"
            / "skills"
            / "analyze-lin-slave-contract"
            / "scripts"
            / "validate_lin_contract.py"
        )
        run_checked(
            [
                sys.executable,
                str(contract_validator),
                str(contract_path),
                "--source-root",
                source_root_value,
            ],
            seed_root,
        )

        emitter = import_emitter(seed_root)
        profile = json.loads(json.dumps(contract.get("host_profile")))
        if not isinstance(profile, dict):
            raise CreateError("contract has no host_profile object")
        profile["source"] = {
            "name": Path(source_root_value).name + " C-code contract",
            "sha256": sha256(contract_path),
            "overlay_sha256": sha256(preset_path) if preset_path else "none",
        }
        require_profile(profile, emitter)
        attach_presets(profile, presets)

        destination_parent = Path(args.destination).resolve()
        destination_parent.mkdir(parents=True, exist_ok=True)
        target = destination_parent / args.project_name
        if target.exists():
            raise CreateError("destination already exists: " + str(target))
        stage = Path(
            tempfile.mkdtemp(
                prefix=".{0}-stage-".format(args.project_name),
                dir=str(destination_parent),
            )
        )
        copy_seed_project(seed_project, stage)

        project_file = stage / (args.project_name + ".pro")
        original_project = stage / "AmbientDevice.pro"
        project_text = original_project.read_text(encoding="utf-8-sig")
        project_text, count = re.subn(
            r"(?m)^\s*TARGET\s*=.*$",
            "TARGET = " + args.project_name,
            project_text,
            count=1,
        )
        if count != 1:
            raise CreateError("mother Seed project has no TARGET assignment")
        original_project.unlink()
        project_file.write_text(project_text, encoding="utf-8", newline="\n")

        shutil.copytree(
            seed_root / "tools",
            stage / "tools",
            ignore=shutil.ignore_patterns("__pycache__", "*.pyc", "*.tmp"),
        )
        for skill_name in ("analyze-lin-slave-contract", "generate-qt-ldf-host"):
            source_skill = seed_root / ".agents" / "skills" / skill_name
            if source_skill.is_dir():
                shutil.copytree(
                    source_skill,
                    stage / ".agents" / "skills" / skill_name,
                    ignore=shutil.ignore_patterns("__pycache__", "*.pyc", "*.tmp"),
                )

        profile_dir = stage / "profiles"
        profile_dir.mkdir(parents=True, exist_ok=True)
        shutil.copy2(
            contract_path,
            profile_dir / (args.project_name + ".code-contract.json"),
        )
        if preset_path is not None:
            shutil.copy2(
                preset_path,
                profile_dir / (args.project_name + ".presets.json"),
            )

        generated_dir = stage / "generated"
        generated_dir.mkdir(parents=True, exist_ok=True)
        report = json.dumps(
            build_report(contract, contract_path, profile),
            ensure_ascii=False,
            indent=2,
        ) + "\n"
        (generated_dir / "linprofile_generated.h").write_text(
            emitter.emit_header(profile), encoding="utf-8", newline="\n"
        )
        (generated_dir / "linprofile_generated.cpp").write_text(
            emitter.emit_source(profile), encoding="utf-8", newline="\n"
        )
        (generated_dir / "linprofile_report.json").write_text(
            report, encoding="utf-8", newline="\n"
        )
        (generated_dir / "linprofile_report.json.txt").write_text(
            report, encoding="utf-8", newline="\n"
        )

        compiler_safe_sources(stage, project_file)
        host_validator = (
            seed_root
            / ".agents"
            / "skills"
            / "generate-qt-ldf-host"
            / "scripts"
            / "validate_generated_host.py"
        )
        run_checked([sys.executable, str(host_validator), str(stage)], stage)

        try:
            os.replace(str(stage), str(target))
        except OSError:
            shutil.copytree(stage, target)
            shutil.rmtree(stage)
        stage = None
        print("created code-contract Qt host: " + str(target))
        print("project file: " + str(target / project_file.name))
        print(
            "profile: {0} nodes, {1} presets, diagnostics={2}".format(
                len(profile["nodes"]),
                len(profile["signal_presets"]),
                profile["models"]["diagnostic"],
            )
        )
        print("Qt compilation skipped by mother Seed policy")
        return 0
    except (CreateError, OSError, TypeError, ValueError) as exc:
        print("project creation failed: " + str(exc), file=sys.stderr)
        return 2
    finally:
        if stage is not None and stage.exists():
            shutil.rmtree(stage, ignore_errors=True)


if __name__ == "__main__":
    sys.exit(main())
