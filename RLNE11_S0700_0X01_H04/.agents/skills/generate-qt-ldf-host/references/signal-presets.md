# Signal-combination overlay

Use a second JSON overlay for user-supplied combinations of any exact LDF
signals. RGB, U/V, CCT, intensity and vendor fields use the same format:

```json
{
  "signal_presets": {
    "group_name": "U/V 快捷组合",
    "items": [
      {
        "name": "UV 01 | u=40 v=94",
        "values": {
          "IntSig_u_01": 40,
          "IntSig_v_01": 94,
          "Intensity_01": 100
        }
      }
    ]
  }
}
```

Rules:

- Item order is visible button order; names are non-empty and unique.
- Allow at most 512 items. The UI shows 30 per page and creates no per-item
  widget in `.ui`.
- Values are decimal integers or strings such as `"0xFF"`.
- Every key exactly matches one signal in any selected master-published frame,
  and every raw value fits the LDF bit width. Ambiguous or absent names fail.
- Never scale, clamp, invert, rename, or silently substitute a supplied value.
- One click atomically applies all listed cross-frame assignments to the shared
  signal snapshot and requests a high-priority refresh through Scheduler and
  the single LIN Worker.
- Omitted signals retain the current value.

Generate with the base overlay first and preset overlay second.
