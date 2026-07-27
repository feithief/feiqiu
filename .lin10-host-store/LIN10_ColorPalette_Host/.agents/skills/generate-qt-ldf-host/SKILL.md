---
name: generate-qt-ldf-host
description: Create an independent Qt LIN upper-computer project from this mother Seed using an LDF, a project name, and optional named combinations of exact LDF signal values. Use when generating a new LDF-based host, cloning the mother Seed without changing it, adding shortcut buttons for multi-signal presets such as RGB colors, or regenerating and checking an existing profile.
---

# Generate Qt LDF Host

Use the current repository as the mother Seed. Keep its UI, scheduler, worker-thread model, diagnostics, debug panel, and generated `LinLayout` architecture. Put all LDF-specific data in JSON overlays and generated files.

## Required inputs

- Require an LDF path and a new project name when creating a project.
- Accept named signal combinations as optional input. Each combination needs only a button name and exact LDF signal/value pairs; do not ask the user to describe UI wiring or C++ behavior.
- If the user supplies slave source code, use it to verify application semantics and proprietary diagnostics. Never infer a proprietary DID from the LDF alone.

## Creating a new project

1. Read [references/seed-contract.md](references/seed-contract.md).
2. Inspect the LDF and the chosen base overlay. If no suitable overlay exists, run the generator's `init` command, then resolve every `TODO` from the LDF, supplied slave code, or explicit user information.
3. If signal combinations were supplied, read [references/signal-presets.md](references/signal-presets.md), preserve their order and values exactly, and write a preset-only JSON overlay.
4. Validate the combinations before copying:

   ```text
   python scripts/validate_signal_presets.py PRESETS.json --ldf INPUT.ldf --overlay BASE.profile.json
   ```

5. Create the independent project:

   ```text
   python scripts/create_qt_host.py --project-name NAME --ldf INPUT.ldf --overlay BASE.profile.json --presets PRESETS.json --destination OUTPUT_PARENT
   ```

   Omit `--presets` when no combinations are needed.

6. Report the new project path, selected profile, preset count, static checks, and any unresolved semantic assumptions. Do not compile Qt; this mother Seed is built inside the user's VM unless the user explicitly changes that instruction.

## Updating an existing project

- Change the JSON overlay, not generated C++.
- Run `tools/ldf_profile_gen.py generate`, then `check`, with the base overlay first and the preset overlay second.
- For signal combinations, validate with `scripts/validate_signal_presets.py` before regeneration.
- Do not edit `.ui` merely to add, remove, reorder, or rename shortcut buttons. The first 30 existing button widgets are configured from generated preset data at runtime.

## Safety and completion rules

- Never overwrite the mother Seed or an existing destination project.
- Never copy `.pro.user`, build directories, `.git`, caches, or temporary files.
- Keep LIN device I/O inside `LinBusWorker`; UI-to-worker calls remain queued.
- Treat exact LDF signal names and bit widths as the source of truth. Reject missing signals, duplicate names, out-of-range values, and combinations outside the primary control frame.
- A generated project is complete only after preset validation, profile generation, and profile `check` all pass.
