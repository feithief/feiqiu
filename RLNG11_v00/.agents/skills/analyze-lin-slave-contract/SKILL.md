---
name: analyze-lin-slave-contract
description: "Read a LIN slave C project without an LDF, extract an evidence-backed LIN contract, and generate or repair an independent mother-Seed Qt host. Covers NAD/address ranges and mappings, application frame/signal bit layout, feedback PID mapping, diagnostic NAD rules, SID/DID services, lengths, byte order, security, and optional reusable signal presets. Use when the only variable input is slave C firmware, when diagnostics/signals fail, or when replacing LDF inference with direct code analysis."
---

# Analyze LIN Slave Contract

Use only the slave source tree. Require generated LIN configuration, application
communication code, and diagnostic/transport code. If any required layer is
missing or inactive build branches cannot be determined, report insufficient
evidence instead of guessing.

## Workflow

1. Read [references/recognition-rules.md](references/recognition-rules.md).
2. Run the deterministic evidence scan:

   ```text
   python scripts/scan_lin_slave.py SLAVE_SOURCE_ROOT --output lin_contract.draft.json
   ```

3. Read only the files and lines cited by the draft, plus the compiler-project
   defines that select active `#if` branches.
4. Replace candidates and `review_required` fields using direct code evidence.
   Produce exactly two reviewed sections:

   - `addressing_contract`: all address domains, valid ranges, conversions, and
     NAD/address/PID relationships.
   - `protocol_contract`: application frames/signals plus diagnostic transport
     and services.

   Follow [references/contract-schema.md](references/contract-schema.md).
5. Mark both sections and `review.status` as `confirmed`, leave
   `review.unresolved` empty, and validate:

   ```text
   python scripts/validate_lin_contract.py lin_contract.json --source-root SLAVE_SOURCE_ROOT
   ```

6. Put the reviewed runtime profile in the contract's `host_profile`, then
   create a new project with the repository renderer:

   ```text
   python tools/create_qt_host_from_contract.py --project-name NAME --contract lin_contract.json --presets OPTIONAL_PRESETS.json --destination OUTPUT_PARENT
   ```

   Omit `--presets` when no combinations were supplied. The command validates
   the source evidence, copies the fixed mother Seed, emits the profile, uses
   readable `.cc` compiler sources, and runs static acceptance. Never overwrite
   an existing destination and never compile Qt on the local machine.
7. Keep scheduler, worker thread, queued synchronization, UI navigation, debug
   panel, feedback-gated diagnostics, and paged preset UI fixed. Never fall
   back to guessed LDF semantics or unvalidated manual generated C++ edits.

## Reliability gates

- Keep diagnostic NAD, application target/mask, group address, and feedback PID
  as separate domains. Record conversions explicitly.
- Derive ranges from active validation code, lookup tables, persisted defaults,
  and array bounds, not from `uint8_t`/`uint16_t` width.
- Derive bit layout from generated accessor macros and actual application reads
  or writes. Do not trust C struct packing or comments alone.
- Trace diagnostics end to end: receive acceptance -> NAD rule ->
  transport/PCI -> SID dispatcher -> DID table -> response builder ->
  response NAD.
- Keep transport acknowledgement separate from persistence verification. For a
  bulk configuration write, finish every write first, wait the confirmed flash
  completion window once, then perform one unified read-back pass.
- Treat inactive `#if` branches, examples, commented code, and generic stack
  capabilities as candidates only.
- Cross-check every contract decision against at least one cited source line;
  require two independent code locations for NAD conversions and diagnostic
  response rules.
- Never enable a diagnostic button or service that the confirmed contract does
  not support.
