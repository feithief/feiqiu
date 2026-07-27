#!/usr/bin/env python3
"""Create a self-contained Qt LIN host from the project-local mother Seed."""

from __future__ import annotations

import argparse
import os
import re
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path
from typing import List, Optional, Sequence


class CreateError(Exception):
    """A concise user-facing creation failure."""


PROJECT_NAME_RE = re.compile(r"^[A-Za-z][A-Za-z0-9_]*$")


def discover_project_source(seed_root: Path) -> Path:
    original = seed_root / "AmbientDevice" / "AmbientDevice.pro"
    if original.is_file():
        return original.parent
    candidates = sorted(seed_root.glob("*.pro"))
    if len(candidates) == 1:
        return seed_root
    raise CreateError(
        "mother Seed must contain AmbientDevice/AmbientDevice.pro or one root .pro file"
    )


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


def copy_project_files(source: Path, target: Path) -> None:
    entries = [entry for entry in source.iterdir() if not should_skip(entry.name)]
    for entry in entries:
        destination = target / entry.name
        if entry.is_dir():
            shutil.copytree(
                entry,
                destination,
                ignore=shutil.ignore_patterns("__pycache__", "*.pyc", "*.tmp"),
            )
        else:
            shutil.copy2(entry, destination)


def copy_file_unique(source: Path, directory: Path, fallback_name: str) -> Path:
    directory.mkdir(parents=True, exist_ok=True)
    name = source.name
    destination = directory / name
    if destination.exists():
        destination = directory / fallback_name
    shutil.copy2(source, destination)
    return destination


def run_checked(command: List[str], cwd: Path) -> None:
    completed = subprocess.run(command, cwd=str(cwd), check=False)
    if completed.returncode != 0:
        raise CreateError(
            "command failed ({0}): {1}".format(
                completed.returncode, " ".join(command)
            )
        )


def build_argument_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(
        description="Create an independent Qt project from the LDF mother Seed."
    )
    parser.add_argument("--project-name", required=True)
    parser.add_argument("--ldf", required=True)
    parser.add_argument("--overlay", required=True, help="reviewed base profile")
    parser.add_argument("--presets", help="optional preset-only signal overlay")
    parser.add_argument("--destination", required=True, help="output parent directory")
    parser.add_argument(
        "--seed-root",
        help="mother Seed root; defaults to the repository containing this skill",
    )
    return parser


def main(argv: Optional[Sequence[str]] = None) -> int:
    args = build_argument_parser().parse_args(argv)
    stage: Optional[Path] = None
    try:
        if not PROJECT_NAME_RE.fullmatch(args.project_name):
            raise CreateError(
                "project name must start with a letter and contain only letters, digits, and underscores"
            )

        seed_root = (
            Path(args.seed_root).resolve()
            if args.seed_root
            else Path(__file__).resolve().parents[4]
        )
        project_source = discover_project_source(seed_root)
        ldf = Path(args.ldf).resolve()
        overlay = Path(args.overlay).resolve()
        presets = Path(args.presets).resolve() if args.presets else None
        for label, path in (("LDF", ldf), ("base profile", overlay)):
            if not path.is_file():
                raise CreateError("{0} does not exist: {1}".format(label, path))
        if presets is not None and not presets.is_file():
            raise CreateError("preset overlay does not exist: " + str(presets))

        destination_parent = Path(args.destination).resolve()
        destination_parent.mkdir(parents=True, exist_ok=True)
        target = destination_parent / args.project_name
        if target.exists():
            raise CreateError("destination already exists: " + str(target))

        skill_root = Path(__file__).resolve().parents[1]
        validator = skill_root / "scripts" / "validate_signal_presets.py"
        if presets is not None:
            run_checked(
                [
                    sys.executable,
                    str(validator),
                    str(presets),
                    "--ldf",
                    str(ldf),
                    "--overlay",
                    str(overlay),
                    "--seed-root",
                    str(seed_root),
                ],
                seed_root,
            )

        stage = Path(
            tempfile.mkdtemp(
                prefix=".{0}-stage-".format(args.project_name),
                dir=str(destination_parent),
            )
        )
        copy_project_files(project_source, stage)

        seed_gitignore = seed_root / ".gitignore"
        if seed_gitignore.is_file() and not (stage / ".gitignore").exists():
            shutil.copy2(seed_gitignore, stage / ".gitignore")

        pro_files = sorted(stage.glob("*.pro"))
        if len(pro_files) != 1:
            raise CreateError("copied project must contain exactly one .pro file")
        project_file = stage / (args.project_name + ".pro")
        pro_text = pro_files[0].read_text(encoding="utf-8")
        pro_text, replacement_count = re.subn(
            r"(?m)^\s*TARGET\s*=.*$",
            "TARGET = " + args.project_name,
            pro_text,
            count=1,
        )
        if replacement_count != 1:
            raise CreateError("source .pro file has no TARGET assignment")
        if project_file != pro_files[0]:
            pro_files[0].unlink()
        project_file.write_text(pro_text, encoding="utf-8", newline="\n")

        source_tools = seed_root / "tools"
        if not (source_tools / "ldf_profile_gen.py").is_file():
            raise CreateError("mother Seed tools/ldf_profile_gen.py is missing")
        shutil.copytree(
            source_tools,
            stage / "tools",
            ignore=shutil.ignore_patterns("__pycache__", "*.pyc", "*.tmp"),
        )
        shutil.copytree(
            skill_root,
            stage / ".agents" / "skills" / skill_root.name,
            ignore=shutil.ignore_patterns("__pycache__", "*.pyc", "*.tmp"),
        )

        target_ldf = copy_file_unique(ldf, stage / "LDF", "network.ldf")
        target_overlay = copy_file_unique(
            overlay, stage / "profiles", args.project_name + ".profile.json"
        )
        target_presets = None
        if presets is not None:
            target_presets = copy_file_unique(
                presets,
                stage / "profiles",
                args.project_name + ".presets.json",
            )

        generator = stage / "tools" / "ldf_profile_gen.py"
        output = stage / "generated"
        overlays = ["--overlay", str(target_overlay)]
        if target_presets is not None:
            overlays.extend(["--overlay", str(target_presets)])
        common = [
            "--ldf",
            str(target_ldf),
            *overlays,
            "--output",
            str(output),
        ]
        run_checked([sys.executable, str(generator), "generate", *common], stage)
        run_checked([sys.executable, str(generator), "check", *common], stage)

        try:
            os.replace(str(stage), str(target))
        except OSError:
            # Windows/VM shared folders may reject an otherwise valid directory
            # rename.  The destination is known not to exist, so copy it as a
            # fallback and remove only this script's private staging directory.
            try:
                shutil.copytree(stage, target)
            except Exception:
                if target.exists():
                    shutil.rmtree(target, ignore_errors=True)
                raise
            shutil.rmtree(stage)
        stage = None
        print("created independent Qt host: " + str(target))
        print("project file: " + str(target / project_file.name))
        print("Qt compilation skipped by mother Seed policy")
        return 0
    except (CreateError, OSError) as exc:
        print("project creation failed: " + str(exc), file=sys.stderr)
        return 2
    finally:
        if stage is not None and stage.exists():
            shutil.rmtree(stage, ignore_errors=True)


if __name__ == "__main__":
    sys.exit(main())
