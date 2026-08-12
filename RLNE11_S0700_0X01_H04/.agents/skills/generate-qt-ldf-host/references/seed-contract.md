# Mother Seed contract

The skill lives at `.agents/skills/generate-qt-ldf-host` and is copied into every
generated project.

Reusable parts:

- `AmbientDevice/`: Qt UI behind `LinRuntime`, scheduler facade, one LIN worker
  thread, replaceable `LinTransport`, write-only `DebugSink`, read-only
  `DebugSnapshotSource`, diagnostics, debug panel, and reusable 30-button
  paged shortcut palette.
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

The generated layout must contain every active master-published signal and
every active slave-published feedback signal. Known optional convenience
semantics may use typed `BCMSignal` members; all remaining master fields,
including U/V and vendor-defined controls, use `ELinSignalRawValue` indexed by
exact source name. RGB is not a required model. The reusable frame-signal page
lists every published frame in a selector, then creates one raw-value editor
for every layout entry in the selected frame. Switching frames rebuilds the
rows from `LinLayout`; applying the selected frame retains every other frame's
stored values. Unknown semantics are displayed raw, never omitted or frozen
invisibly in the default payload.

Named combinations may span multiple published frames. Each generated
assignment stores its exact frame index, signal name, bit geometry and raw
value. Applying a combination updates one `BCMSignal` snapshot and queues every
editable published frame in the single LIN Worker. Fixed RGB/address/intensity
widgets hide when their typed signals are absent; adding a signal never requires
a Designer-file change.

Diagnostic navigation is feedback-gated. A node starts disabled and becomes
clickable only after a fully validated status-frame response. A timeout, PID
mismatch, checksum failure, driver error, or malformed payload disables it
again. Keep this rule in reusable Seed code, not in generated profile data.

Do not run qmake, make, or a compiler in this workflow. Keep shortcut sends on
the queued Scheduler-to-Worker path. `LIN.ActiveSignalPreset` records the active
shortcut, while `Reserved.01` through `Reserved.10` remain available for future
features.

The dependency direction is fixed:

`QWidget -> LinRuntime -> AmbientLinScheduler -> LinBusWorker -> LinTransport`.

Only `main.cpp` knows concrete scheduler and transport types. UI modules must
not include `ambientlinscheduler.h`. All serial access is Worker-owned; only
`DebugStore` may contain a mutex, and its values are observability-only.
