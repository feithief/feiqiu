---
name: generate-qt-ldf-host
description: Create a new independent Qt LIN upper-computer from the mother Seed using an LDF, a project name, and optional named combinations of any exact LDF signal values. Treats RGB, U/V, CCT, intensity, modes, address fields, and unknown vendor fields uniformly; preserves every active master-control and slave-feedback signal. Use for quickly generating or regenerating an LDF-based host, paged cross-frame multi-signal shortcuts, dynamic signal UI, and static acceptance without compiling Qt.
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
   For indexed signal families, keep the user's compact table as data and run:

   ```text
   python scripts/build_signal_family_presets.py FAMILY_TABLE.json PRESETS.json
   ```
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
- For custom-DID profiles, keep the page hidden and retry the complete initial
  configuration read until it succeeds. Show the page only from a
  `configurationReady` signal after all required values are populated; never
  expose reset zeros. The 5000 ms valid-node-response watchdog remains active
  during retries. Status-only profiles may signal ready immediately.
- Before encoding editable diagnostic values, call `interpretText()` on every
  `QSpinBox` and `QDoubleSpinBox`, including the last field touched by the
  on-screen keyboard. Round scaled fixed-point values before little-endian
  encoding instead of truncating them.
- When the profile defines reviewed custom-DID services, enable read/write and
  calibration controls.
- When the LDF defines only standard LIN node configuration or no proprietary
  services, an online node may open the same page in status-only mode. Disable
  write/calibration controls and send no guessed proprietary request.
- Do not close the page merely because a node becomes offline.
- Show only read/write success popups. Route progress and failures to Debug.
- Build Status Report rows from every signal in the active slave-published
  frame. Show shortened source names and raw hexadecimal values; never collapse
  fields into six fixed error slots or translate them to semantic text.
- Accept raw status fields that have no normal/error semantics. The runtime
  validator must exempt `ELinStatusRawValue` from the
  `normalValue != errorValue` rule while retaining name, enum, bit-width, and
  payload-range checks. Repeated raw fields are valid and must not block startup.
- Treat `Locked` and `Unlocked` as non-clickable state indicators sourced only
  from readable DID `0x0002` bit7. Put separate `Lock` and `Unlock` action
  buttons below them. After either manual action, wait 1000 ms, issue `0x22`
  for DID `0x0002`, and update/report success only from that read-back.
- A host is incomplete if `validate_generated_host.py` fails any navigation,
  source, report, C++ literal, or Qt macro-collision check.

## Mandatory complete-signal contract

- The reusable communication core is signal-generic. RGB, U/V, CCT and every
  vendor-specific field are ordinary `(frame, signal, start bit, width, raw
  value)` entries. Never require RGB semantics merely because the application
  controls light. Use `models.color=generic_signals` and
  `models.addressing=implicit_per_signal` when the LDF selects targets through
  per-node frames or suffixed signal families.
- Preserve every signal in every selected master-published LDF frame. Keep
  recognized lighting controls as typed bindings; automatically emit every
  unrecognized field as `ELinSignalRawValue` with its exact LDF name, start bit,
  width, and initial value. Never turn an unknown field into an invisible static
  payload bit.
- Generate an editable, scrollable all-signal page grouped by frame. Enumerate
  `publishedFrames[0..n]` and each frame's `signalLayouts[0..n]`; never hard-code
  RGB, brightness, one primary-frame-only list, or a maximum visible row count.
- Treat that page as a reusable control pool: create/show one row for every
  configured signal and create no row for unused capacity. Fixed convenience
  controls stay reserved in the mother Seed but hide automatically when their
  typed signal is absent. A new layout must not require a `.ui` edit.
- Store raw master values by exact source signal name in the shared immutable
  `BCMSignal` snapshot. Encode them only inside the layout/Worker path, and keep
  the original RGB page synchronized after an all-signal apply.
- Preserve every signal in every active slave-published frame as raw status
  rows. Exclude only frames proven inactive by the chosen build/profile.
- Do not deduplicate status layouts only by bit geometry. Include the exact
  source signal name in the layout signature so one node never displays
  another node's suffixed names merely because their bits are equal.
- Statically compare source LDF/contract signal sets with generated bindings and
  status layouts. Missing, duplicated, overlapping, wider-than-32-bit, or
  out-of-payload signals fail generation; they must never be silently skipped.

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
- Generate a useful power-up payload through the extracted signal layout. For
  typed legacy profiles retain the proven target/enable/intensity/RGB policy.
  For generic profiles use exact `signal_defaults` from LDF/firmware evidence
  and list activation-critical fields in
  `startup_required_nonzero_signals`; reject any required field left at zero.
- Reject missing signals, duplicate/ambiguous names and out-of-range values.
  A preset may atomically assign signals in any selected master-published frame;
  store its generated frame index plus exact name/geometry and send every
  editable frame through the Worker priority path. Never restrict combinations
  to the primary frame.
- Support up to 512 preset combinations with 30 buttons per page. Do not add
  fixed buttons to `.ui` for every combination.
- If DLP protection replaces a generated text file with a binary wrapper, use
  its executable/readable `.txt` companion and ensure the `.pro` file lists
  readable compiler sources.
- Keep both arguments of Qt `qMin`/`qMax` the same C++ type. Cast anonymous
  enum limits such as `LinMaximumStatusFields` to `int`; the Qt 5 ARM compiler
  rejects mixed `int`/enum template deduction.
- Statically reject a Seed/host whose generated profile can emit
  `ELinStatusRawValue` but whose runtime layout validation still applies the
  semantic normal/error inequality to that raw field.
