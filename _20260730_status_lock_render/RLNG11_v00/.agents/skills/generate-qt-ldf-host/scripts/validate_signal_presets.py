#!/usr/bin/env python3
"""Validate a signal-preset overlay alone or against an LDF/base profile."""

from __future__ import annotations

import argparse
import importlib.machinery
import importlib.util
import json
import sys
from pathlib import Path
from typing import Any, Dict, List, Optional, Sequence, Tuple


class PresetError(Exception):
    """A concise user-facing validation failure."""


def load_json_object(path: Path) -> Dict[str, Any]:
    try:
        value = json.loads(path.read_text(encoding="utf-8"))
    except json.JSONDecodeError as exc:
        raise PresetError(
            "{0}:{1}:{2}: invalid JSON: {3}".format(
                path, exc.lineno, exc.colno, exc.msg
            )
        )
    except OSError as exc:
        raise PresetError("cannot read {0}: {1}".format(path, exc))
    if not isinstance(value, dict):
        raise PresetError("preset file root must be a JSON object")
    return value


def parse_raw_integer(value: Any, path: str) -> int:
    if isinstance(value, bool):
        raise PresetError(path + " must be an integer, not true/false")
    if isinstance(value, int):
        result = value
    elif isinstance(value, str):
        try:
            result = int(value.strip(), 0)
        except ValueError:
            raise PresetError(path + " must be a decimal or 0x-prefixed integer")
    else:
        raise PresetError(path + " must be an integer")
    if result < 0:
        raise PresetError(path + " must not be negative")
    return result


def validate_shape(document: Dict[str, Any]) -> Tuple[int, int]:
    if set(document) != {"signal_presets"}:
        raise PresetError(
            "preset-only overlay must contain exactly the 'signal_presets' key"
        )
    section = document["signal_presets"]
    if not isinstance(section, dict):
        raise PresetError("signal_presets must be an object")
    unknown = set(section) - {"group_name", "items"}
    if unknown:
        raise PresetError(
            "unknown signal_presets keys: " + ", ".join(sorted(unknown))
        )
    group_name = section.get("group_name", "Signal Presets")
    if not isinstance(group_name, str) or not group_name.strip():
        raise PresetError("signal_presets.group_name must be a non-empty string")
    items = section.get("items")
    if not isinstance(items, list):
        raise PresetError("signal_presets.items must be an array")
    if not items:
        raise PresetError("signal_presets.items must not be empty")
    if len(items) > 512:
        raise PresetError("signal_presets.items may contain at most 512 buttons")

    used_names = set()
    assignment_count = 0
    for index, item in enumerate(items):
        item_path = "signal_presets.items[{0}]".format(index)
        if not isinstance(item, dict):
            raise PresetError(item_path + " must be an object")
        if set(item) != {"name", "values"}:
            raise PresetError(item_path + " must contain exactly name and values")
        name = item["name"]
        if not isinstance(name, str) or not name.strip():
            raise PresetError(item_path + ".name must be a non-empty string")
        if name in used_names:
            raise PresetError("duplicate preset name: " + name)
        used_names.add(name)
        values = item["values"]
        if not isinstance(values, dict) or not values:
            raise PresetError(item_path + ".values must be a non-empty object")
        for signal_name, raw_value in values.items():
            if not isinstance(signal_name, str) or not signal_name:
                raise PresetError(item_path + ".values keys must be signal names")
            parse_raw_integer(raw_value, item_path + ".values." + signal_name)
            assignment_count += 1
    return len(items), assignment_count


def load_generator(seed_root: Path):
    candidates = [
        seed_root / "tools" / "ldf_profile_gen.py",
        seed_root / "tools" / "ldf_profile_gen.py.txt",
    ]
    generator_path = None
    for candidate in candidates:
        if not candidate.is_file():
            continue
        try:
            candidate.read_text(encoding="utf-8-sig")
            generator_path = candidate
            break
        except UnicodeDecodeError:
            continue
    if generator_path is None:
        raise PresetError(
            "mother Seed has no readable tools/ldf_profile_gen.py or .py.txt"
        )
    loader = importlib.machinery.SourceFileLoader(
        "seed_ldf_profile_gen", str(generator_path)
    )
    spec = importlib.util.spec_from_loader(loader.name, loader)
    if spec is None or spec.loader is None:
        raise PresetError("cannot load mother Seed generator: " + str(generator_path))
    module = importlib.util.module_from_spec(spec)
    sys.modules[spec.name] = module
    spec.loader.exec_module(module)
    return module


def build_argument_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(
        description="Validate named combinations of exact LDF signal values."
    )
    parser.add_argument("preset", help="preset-only JSON overlay")
    parser.add_argument("--ldf", help="LDF used for exact signal/width validation")
    parser.add_argument("--overlay", help="base profile merged before the preset")
    parser.add_argument(
        "--seed-root",
        help="mother Seed root; defaults to the repository containing this skill",
    )
    return parser


def main(argv: Optional[Sequence[str]] = None) -> int:
    args = build_argument_parser().parse_args(argv)
    try:
        preset_path = Path(args.preset).resolve()
        document = load_json_object(preset_path)
        preset_count, assignment_count = validate_shape(document)

        if bool(args.ldf) != bool(args.overlay):
            raise PresetError("--ldf and --overlay must be provided together")
        if args.ldf:
            seed_root = (
                Path(args.seed_root).resolve()
                if args.seed_root
                else Path(__file__).resolve().parents[4]
            )
            generator = load_generator(seed_root)
            network = generator.parse_ldf(Path(args.ldf).resolve())
            merged, overlay_sha = generator.load_overlays(
                [Path(args.overlay).resolve(), preset_path]
            )
            profile = generator.build_profile(network, merged, overlay_sha)
            preset_count = len(profile["signal_presets"])
            assignment_count = sum(
                len(item["assignments"]) for item in profile["signal_presets"]
            )

        print(
            "signal presets valid: {0} buttons, {1} assignments".format(
                preset_count, assignment_count
            )
        )
        return 0
    except PresetError as exc:
        print("preset validation failed: " + str(exc), file=sys.stderr)
        return 2
    except Exception as exc:
        # Preserve the generator's concise validation message without a traceback.
        print("preset validation failed: " + str(exc), file=sys.stderr)
        return 2


if __name__ == "__main__":
    sys.exit(main())
