# Code recognition rules

## Evidence priority

Use this order:

1. Active generated LIN configuration and access macros.
2. Active application communication/diagnostic call paths.
3. Persisted configuration initialization and validation.
4. Generic LIN stack implementation.
5. Comments, examples, and inactive conditional branches.

Inspect compiler defines in `.uvprojx`, CMake, Makefile, or IDE project files
before accepting code guarded by `#if`/`#ifdef`.

## Address domains

Never merge these merely because the code calls each one “NAD”:

- `diagnostic_initial_nad`: factory/boot address used by assignment services.
- `diagnostic_current_nad`: transport address accepted by MasterReq and returned
  in SlaveResp.
- `diagnostic_special_nad`: calibration, functional, broadcast, or temporary
  address with restricted services.
- `application_target_or_mask`: ordinary control-frame field selecting lamps.
- `group_address`: ordinary control selection, not necessarily diagnostic NAD.
- `feedback_frame_id_or_pid`: status header selected from an address/PID table.

Trace initialization, update, range check, storage, request acceptance, and
response construction for each domain.

## Application frames

Generated macros such as `l_*_rd_*` mean the slave consumes a master frame;
`l_*_wr_*` means the slave publishes a status field. Extract:

- frame name and direction;
- raw ID/PID and checksum when active code exposes them;
- byte index, shift, mask, start bit, and bit width;
- default payload;
- actual semantic use in application code;
- address or PID conversion applied before/after the frame.

For multi-byte fields, derive endianness from shift/or expressions or sequential
accesses. Do not infer endianness from a variable type.

## Diagnostics

Trace the active path rather than searching isolated constants:

1. MasterReq NAD acceptance.
2. PCI/single/multi-frame transport.
3. SID dispatcher.
4. DID/subfunction table and read/write/NAD permissions.
5. positive/negative response layout.
6. SlaveResp NAD rule.
7. delay, queue, unlock, and post-write constraints.

For flash-backed bulk configuration, do not treat a positive write response as
proof that persistence has completed. Record a bulk-write/read-back policy:
send the complete batch, wait one evidence-backed flash window, then read back
the batch. Never insert a speculative read between individual writes.

Differentiate standard LIN node configuration services from proprietary
SID `0x22`, `0x2E`, `0x27`, or project-specific services. For SecurityAccess,
derive the seed byte order and the exact key payload byte count from every byte
read by the active key handler; never infer key width from the arithmetic
constant or clock source type. A DID table alone does
not prove the transport framing or the response NAD.

The mother Seed intentionally narrows SecurityAccess after confirming support:

- never call SID `0x27` from page entry, `0x22`, `0x2E`, or calibration;
- expose one manual `Unlock` action only, with four little-endian key bytes
  calculated as `seed + 0x0C04`;
- expose one manual `Lock` action only when writable DID `0x0002` has length
  two, sending data bytes `82 00`;
- render `Locked` and `Unlocked` as separate, non-clickable state indicators
  sourced from readable DID `0x0002` bit7; place the `Lock` and `Unlock`
  action buttons beneath them and never generate a separate unlock screen;
- after either action, wait 1000 ms, issue `0x22` for DID `0x0002`, and accept
  success only when bit7 equals the requested state.

Treat node liveness separately from a single frame type. A diagnostic page's
5000 ms single-shot watchdog must be renewed by either a valid status response
or a valid diagnostic response for that same node. Display every field in the
active slave-published frame as short source signal names plus raw hexadecimal
values. Index by layout position so two raw fields cannot collapse into one
semantic slot; semantic error text is debug interpretation, not the page value.

For the primary control frame, generate an immediately visible power-up
payload from the extracted signal layout: select all one-hot target masks (or
the first value-addressed node), set LED/direct-RGB enable and validity bits
when present, set confirmed non-zero intensity, and use full-scale direct RGB
white. Preserve unrelated static bits. Reject a profile whose startup target
or intensity is zero.

For a multi-frame `0x2E`, compare the buffer used to decode the DID with the
object used to build the positive response. If the handler decodes the DID from
the accumulated transport buffer but echoes bytes from the current/final
consecutive frame, record
`write_completion_policy=send_only_then_delayed_0x22_readback` with evidence
from both the transport call site and response builder. Set writable host
services to `expect_positive_write_response=false`: `0x2E` only sends. Finish
every write, wait the confirmed flash window once, then let `0x22` only read
and compare; all values must match before reporting success. Keep the default
positive-response evidence in the reviewed contract when useful, but the
mother-Seed host must not wait for `0x6E`.

Before a Qt write snapshot, force every spin-box editor to interpret its current
text. This is mandatory for the final field edited with an on-screen keyboard.
Convert decimal fixed-point values by rounding after scaling; do not truncate.

For custom-DID page entry, do not show reset/default fields. Keep the page
hidden, retry the complete configuration read, and reveal it only after every
required DID has succeeded. Continue applying the 5000 ms valid-node-response
watchdog while retrying; a status-only page may open from validated feedback.

## Stop conditions

Leave the contract unconfirmed when:

- generated access macros are absent;
- the active build variant is unknown and changes NAD or diagnostic behavior;
- request and response NAD rules disagree across active code;
- a frame field has unresolved width/offset/endianness;
- only comments or generic library code support a required behavior.
