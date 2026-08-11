#!/usr/bin/env python3
"""Expand compact indexed signal-family combinations into a preset overlay."""

from __future__ import annotations

import argparse
import json
import sys
from pathlib import Path
from typing import Any, Dict, List


class FamilyPresetError(Exception):
    pass


def load_object(path: Path) -> Dict[str, Any]:
    value = json.loads(path.read_text(encoding="utf-8"))
    if not isinstance(value, dict):
        raise FamilyPresetError("input root must be an object")
    return value


def integer(value: Any, path: str) -> int:
    if isinstance(value, bool) or not isinstance(value, int) or value < 0:
        raise FamilyPresetError(path + " must be a non-negative integer")
    return value


def main() -> int:
    parser = argparse.ArgumentParser(
        description="Expand any indexed LDF signal family into exact presets."
    )
    parser.add_argument("input")
    parser.add_argument("output")
    args = parser.parse_args()
    try:
        source = load_object(Path(args.input))
        group_name = source.get("group_name")
        if not isinstance(group_name, str) or not group_name.strip():
            raise FamilyPresetError("group_name must be non-empty")
        start = integer(source.get("index_start"), "index_start")
        end = integer(source.get("index_end"), "index_end")
        if start > end:
            raise FamilyPresetError("index_start must not exceed index_end")
        templates = source.get("value_signal_templates")
        constants = source.get("constant_signal_templates", {})
        items = source.get("items")
        if not isinstance(templates, dict) or not templates:
            raise FamilyPresetError("value_signal_templates must be non-empty")
        if not isinstance(constants, dict):
            raise FamilyPresetError("constant_signal_templates must be an object")
        if not isinstance(items, list) or not items:
            raise FamilyPresetError("items must be a non-empty array")

        output_items: List[Dict[str, Any]] = []
        used_names = set()
        for item_index, item in enumerate(items):
            if not isinstance(item, dict):
                raise FamilyPresetError("items[{0}] must be an object".format(item_index))
            name = item.get("name")
            if not isinstance(name, str) or not name.strip() or name in used_names:
                raise FamilyPresetError("item names must be non-empty and unique")
            used_names.add(name)
            values: Dict[str, int] = {}
            for index in range(start, end + 1):
                for value_key, template in templates.items():
                    if not isinstance(value_key, str) or not isinstance(template, str):
                        raise FamilyPresetError("value signal templates must be strings")
                    if value_key not in item:
                        raise FamilyPresetError(name + " has no value for " + value_key)
                    values[template.format(index=index)] = integer(
                        item[value_key], name + "." + value_key
                    )
                for template, raw_value in constants.items():
                    if not isinstance(template, str):
                        raise FamilyPresetError("constant signal templates must be strings")
                    values[template.format(index=index)] = integer(
                        raw_value, "constant_signal_templates." + template
                    )
            output_items.append({"name": name, "values": values})

        output = {
            "signal_presets": {
                "group_name": group_name,
                "items": output_items,
            }
        }
        Path(args.output).write_text(
            json.dumps(output, ensure_ascii=False, indent=2) + "\n",
            encoding="utf-8",
        )
        print(
            "expanded {0} presets across {1} signal indexes".format(
                len(output_items), end - start + 1
            )
        )
        return 0
    except (OSError, json.JSONDecodeError, KeyError, ValueError, FamilyPresetError) as exc:
        print("family preset expansion failed: " + str(exc), file=sys.stderr)
        return 2


if __name__ == "__main__":
    sys.exit(main())
