# Reviewed contract schema

Keep the scanner's top-level structure and replace candidates with reviewed
values. The generator consumes only `addressing_contract` and
`protocol_contract`; `review` and evidence prevent unsafe generation.

```json
{
  "schema_version": 1,
  "addressing_contract": {
    "status": "confirmed",
    "domains": [
      {
        "name": "diagnostic_current_nad",
        "valid_values": {"minimum": 1, "maximum": 15},
        "source": "saved_single_address",
        "meaning": "MasterReq/SlaveResp transport NAD",
        "evidence": [
          {"file": "user/lin_app.c", "line": 321, "contains": "return g_u8_SlaveNodeNAD;"}
        ]
      },
      {
        "name": "application_target_or_mask",
        "encoding": "little_endian_u16",
        "valid_values": {"table": [1, 2, 4, 8]},
        "meaning": "ordinary control-frame lamp selection",
        "evidence": []
      }
    ],
    "mappings": [
      {
        "from": "diagnostic_current_nad",
        "to": "feedback_frame_id_or_pid",
        "method": "lookup_table",
        "pairs": [{"input": 1, "output": 193}],
        "evidence": []
      }
    ]
  },
  "protocol_contract": {
    "status": "confirmed",
    "frames": [
      {
        "name": "ControlFrame",
        "direction": "master_to_slave",
        "frame_id_or_pid": 36,
        "length": 8,
        "signals": [
          {
            "signal": "Enable",
            "semantic": "led_enable",
            "start_bit": 17,
            "bit_length": 1,
            "default": 0,
            "evidence": []
          }
        ]
      }
    ],
    "diagnostics": {
      "enabled": true,
      "request_frame": {"id": 60, "name": "MasterReq"},
      "response_frame": {"id": 61, "name": "SlaveResp"},
      "request_nad_rule": "current, initial, or broadcast according to active transport branch",
      "response_nad_rule": "current NAD unless assignment service explicitly returns old NAD",
      "bulk_write_readback_delay_ms": 1000,
      "write_completion_policy": "positive_response",
      "write_response_quirk": null,
      "security": {
        "request_seed_sub_function": 3,
        "send_key_sub_function": 4,
        "seed_byte_order": "little_endian_u32",
        "key_rule": "key = seed + 0x0C04",
        "key_addend": 3076,
        "key_length": 4,
        "manual_trigger_only": true,
        "lock_service_id": 2,
        "lock_payload": [130, 0]
      },
      "diagnostic_page_response_watchdog_ms": 5000,
      "status_display": "short_signal_name_and_raw_hex",
      "services": [],
      "evidence": []
    }
  },
  "review": {"status": "confirmed", "unresolved": []}
}
```

`write_completion_policy` defaults to `positive_response`. Use
`send_only_then_delayed_0x22_readback` only for a source-confirmed `0x2E`
response defect. In that profile, every writable service sets
`expect_positive_write_response=false`: `0x2E` only sends. After the complete
batch and flash delay, `0x22` only reads and all readable values must match.
Add a `write_response_quirk` object with
`"applies_to": "multi_frame_0x2E"` plus evidence for the accumulated DID buffer,
final-frame request object, and response echo assignments.

Use symbolic lengths only in the draft. Resolve them from active `#define`
values before confirmation. Every signal and address domain needs source
evidence. Diagnostics need evidence for both acceptance and response paths.
SecurityAccess also needs direct evidence for seed byte order and the exact
number of key bytes consumed by the active handler. The host renderer accepts
only the fixed mother-Seed manual rule: four little-endian key bytes from
`seed + 0x0C04`, Lock DID `0x0002` payload `82 00`, and a 5000 ms page
response watchdog.

For each confirmed `master_to_slave` frame, the flattened union of
`host_profile.published_frames[].bindings[].source_signals` must equal the
complete active source-signal set. Use typed semantics for recognized controls;
use `{"semantic":"raw","enum":"ELinSignalRawValue"}` for every remaining
field while preserving its exact name, bit position, width, and default. Apply
the same completeness rule to slave feedback fields through `status_layouts`.
