# Signal-combination overlay

Use a small second JSON overlay when the user provides named combinations. It is merged after the main profile, so the user only has to supply shortcut data.

```json
{
  "signal_presets": {
    "group_name": "RGB 30色",
    "items": [
      {
        "name": "红色",
        "values": {
          "CDCU_SigEnable_ColorProtocol_CLIN1": 1,
          "CDCU_SigColorR_Predef_CLIN1": 255,
          "CDCU_SigColorG_CLIN1": 0,
          "CDCU_SigColorB_CLIN1": 0
        }
      },
      {
        "name": "绿色",
        "values": {
          "CDCU_SigEnable_ColorProtocol_CLIN1": 1,
          "CDCU_SigColorR_Predef_CLIN1": 0,
          "CDCU_SigColorG_CLIN1": 255,
          "CDCU_SigColorB_CLIN1": 0
        }
      }
    ]
  }
}
```

Rules:

- `items` order is the visible button order. Names must be non-empty and unique.
- Support at most 30 buttons, matching the mother Seed's reusable button pool.
- `values` accepts decimal integers or strings such as `"0xFF"`.
- Every key must exactly match a signal in `schedule.control_frame` of the merged base profile.
- Every raw value must fit the signal's LDF bit width. Do not scale, clamp, invert, or silently substitute values.
- One click applies all listed values to the cached primary-frame payload, then requests a high-priority transmission through the scheduler and LIN worker thread.
- Signals omitted by a combination retain their current primary-frame value. A later ordinary UI edit rebuilds the normal logical payload and supersedes the preset.

Generate with the base profile first and this file second:

```text
python tools/ldf_profile_gen.py generate --ldf INPUT.ldf --overlay profiles/base.profile.json --overlay profiles/name.presets.json --output generated
```
