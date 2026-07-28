---
name: analyze-lin-slave-contract
description: "Read a LIN slave C project without an LDF and extract an evidence-backed contract for the mother Qt Seed: NAD/address ranges and mappings, application frame/signal bit layout, feedback PID mapping, diagnostic request/response NAD rules, SID/DID services, lengths, byte order, and security flow. Use when creating or repairing a Qt LIN upper-computer from slave firmware, checking why diagnostics or signals fail, or replacing LDF-based inference with direct C-code analysis."
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

6. Use the confirmed contract as the only variable input for the fixed mother
   Seed. Do not change scheduler, worker thread, UI navigation, debug panel, or
   synchronization architecture. If the current generator has no code-contract
   renderer, add or reuse one; never fall back to guessed LDF semantics or
   unvalidated manual C++ edits.

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
- Treat inactive `#if` branches, examples, commented code, and generic stack
  capabilities as candidates only.
- Cross-check every contract decision against at least one cited source line;
  require two independent code locations for NAD conversions and diagnostic
  response rules.
- Never enable a diagnostic button or service that the confirmed contract does
  not support.
