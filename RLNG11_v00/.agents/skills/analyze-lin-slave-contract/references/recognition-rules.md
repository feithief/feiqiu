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

## Stop conditions

Leave the contract unconfirmed when:

- generated access macros are absent;
- the active build variant is unknown and changes NAD or diagnostic behavior;
- request and response NAD rules disagree across active code;
- a frame field has unresolved width/offset/endianness;
- only comments or generic library code support a required behavior.
