# Mother Seed contract

The skill lives at `.agents/skills/generate-qt-ldf-host` and is copied into every
generated project.

Reusable parts:

- `AmbientDevice/`: Qt UI, scheduler, one LIN worker thread, diagnostics, debug
  panel, and reusable 30-button paged shortcut palette.
- `tools/ldf_profile_gen.py`: LDF plus ordered JSON overlays to fixed generated
  `LinLayout` C++.
- `profiles/`: reviewed application-semantic overlays.
- `AmbientDevice/generated/`: generated data; never edit manually.

An independent output contains its renamed `.pro`, Qt sources, `generated/`,
`LDF/`, `profiles/`, `tools/`, and this skill. Exclude `.pro.user`, Makefiles,
build output, version-control data, caches, and temporary files.

Creation is complete only after:

1. signal combinations validate;
2. profile `generate` passes;
3. profile `check` passes;
4. `validate_generated_host.py` passes.

Do not run qmake, make, or a compiler in this workflow. Keep shortcut sends on
the queued Scheduler-to-Worker path. `LIN.ActiveSignalPreset` records the active
shortcut, while `Reserved.01` through `Reserved.10` remain available for future
features.
