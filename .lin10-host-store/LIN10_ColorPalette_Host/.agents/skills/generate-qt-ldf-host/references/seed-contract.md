# Mother Seed contract

The skill lives inside the mother Seed at `.agents/skills/generate-qt-ldf-host` and is copied into each generated project.

Seed components:

- `AmbientDevice/`: Qt application source, reusable `.ui` files, scheduler, LIN worker, diagnostics, and debug panel.
- `tools/ldf_profile_gen.py`: parses LDF plus one or more JSON overlays and emits fixed-name C++ layout data.
- `profiles/`: reviewed LDF/application semantic overlays. Generated projects receive only the selected profile and optional preset overlay.
- `AmbientDevice/generated/`: generated `LinLayout` files; never edit these manually.

The destination project is self-contained:

```text
ProjectName/
  ProjectName.pro
  generated/
  LDF/
  profiles/
  tools/
  .agents/skills/generate-qt-ldf-host/
  ...Qt source and UI files...
```

Creation rules:

- The output parent must already be chosen, and `ProjectName` must not exist there.
- Rename `AmbientDevice.pro` to `ProjectName.pro` and set `TARGET = ProjectName`.
- Copy source files, resources, generator, selected LDF, selected overlays, and this skill.
- Exclude `.pro.user`, `Makefile`, build output, version-control data, caches, and temporary files.
- Generate and statically check the profile inside the new project. Do not run qmake, make, or a compiler for this workflow.
- Keep shortcut sends on the existing queued Scheduler-to-Worker path. `LIN.ActiveSignalPreset` is a dedicated debug value; the ten `Reserved.01` through `Reserved.10` variables remain free for future features.
