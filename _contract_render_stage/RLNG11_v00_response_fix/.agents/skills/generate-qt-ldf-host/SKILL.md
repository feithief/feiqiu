---
name: generate-qt-ldf-host
description: Create a new independent Qt LIN upper-computer from the mother Seed using an LDF, a project name, and optional named combinations of exact LDF signal values. Use for quickly generating or regenerating an LDF-based host, adding paged multi-signal shortcut buttons such as RGB colors, and statically accepting diagnostics, UI navigation, generated sources, and Qt-macro safety without compiling Qt.
---

# Generate Qt LDF Host

Generate from the current mother Seed. Keep its injected `LinRuntime` UI
boundary, scheduler facade, single LIN worker thread, replaceable
`LinTransport`, write-only `DebugSink`, read-only `DebugSnapshotSource`,
queued value synchronization,
diagnostics, debug panel, and generated `LinLayout` architecture. Keep
LDF-specific facts in JSON overlays and generated files.

## Inputs

- For a new host, require an LDF path and a new project name.
- Treat named signal combinations as optional data: button name plus exact LDF
  signal/value pairs. Do not ask the user to design C++ or UI wiring.
- Use supplied slave code to verify proprietary diagnostics. Never invent a DID,
  SID, byte layout, scaling rule, or signal meaning from an LDF alone.

## Fast creation workflow

1. Read [references/seed-contract.md](references/seed-contract.md).
2. Reuse a reviewed base overlay when it matches the LDF. Otherwise run the
   generator `init` command and resolve every `TODO` from the LDF, slave code, or
   explicit user input.
3. If combinations were supplied, read
   [references/signal-presets.md](references/signal-presets.md), preserve order
   and raw values, and write a preset-only overlay.
4. Validate combinations:

   ```text
   python scripts/validate_signal_presets.py PRESETS.json --ldf INPUT.ldf --overlay BASE.profile.json
   ```

5. Create the independent project:

   ```text
   python scripts/create_qt_host.py --project-name NAME --ldf INPUT.ldf --overlay BASE.profile.json --presets PRESETS.json --destination OUTPUT_PARENT
   ```

   Omit `--presets` when absent. The creation command generates, checks, and
   statically accepts the host before publishing the destination directory.
6. For an existing project, regenerate with base overlay first and preset
   overlay second, run generator `check`, then run:

   ```text
   python .agents/skills/generate-qt-ldf-host/scripts/validate_generated_host.py PROJECT_ROOT
   ```

7. Report project path, profile, node and preset counts, diagnostic mode,
   validation result, and unresolved semantic assumptions. Do not compile Qt;
   this project is compiled in the user's VM.

## Mandatory diagnostics contract

- Initialize every generated node button disabled.
- Enable a node button only after its configured status frame has been received
  with the expected PID, payload length, driver status, and checksum.
- Disable the node button again on a missing, timed-out, malformed, or invalid
  status response. Do not treat a transmitted header as feedback.
- Gate navigation twice: disable the button in `SlaveButton`, and recheck the
  latest feedback state in `MainWindow::slaveConfig` before opening the page.
- When the profile defines reviewed custom-DID services, enable read/write and
  calibration controls.
- When the LDF defines only standard LIN node configuration or no proprietary
  services, an online node may open the same page in status-only mode. Disable
  write/calibration controls and send no guessed proprietary request.
- Do not close the page merely because a node becomes offline.
- Show only read/write success popups. Route progress and failures to Debug.
- A host is incomplete if `validate_generated_host.py` fails any navigation,
  source, report, C++ literal, or Qt macro-collision check.

## Speed and safety rules

- Work from overlays and generator scripts; do not hand-edit generated C++.
- Do not reopen or analyze unrelated project files after a matching reviewed
  profile is found.
- Never overwrite the mother Seed or an existing destination project.
- Never copy `.pro.user`, build directories, version-control data, caches, or
  temporary files.
- Keep device I/O in `LinBusWorker`; UI-to-worker calls remain queued.
- Keep concrete construction in `main.cpp`. UI feature modules may depend only
  on `LinRuntime`, never `AmbientLinScheduler` or the transport.
- Keep the serial descriptor in one `LinTransport` created, used, and destroyed
  by the Worker thread. Preserve facade, Worker, and transport thread assertions.
- Do not add a business-state mutex. Only `DebugStore` may use `QMutex`, and
  debug values must never control runtime behaviour.
- Preserve F12 GUI/LIN thread IDs, diagnostic queue depth, and active request ID.
- Reject missing signals, duplicate names, out-of-range values, and assignments
  outside the primary control frame.
- Support up to 512 preset combinations with 30 buttons per page. Do not add
  fixed buttons to `.ui` for every combination.
- If DLP protection replaces a generated text file with a binary wrapper, use
  its executable/readable `.txt` companion and ensure the `.pro` file lists
  readable compiler sources.
