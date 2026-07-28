# Signal-combination overlay

Use a second JSON overlay for user-supplied combinations:

```json
{
  "signal_presets": {
    "group_name": "RGB 快捷颜色",
    "items": [
      {
        "name": "红色",
        "values": {
          "ColorProtocolEnable": 1,
          "RGB_R": 255,
          "RGB_G": 0,
          "RGB_B": 0
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
- Every key exactly matches a signal in the merged profile's primary control
  frame, and every raw value fits the LDF bit width.
- Never scale, clamp, invert, rename, or silently substitute a supplied value.
- One click atomically applies all listed assignments to the cached control
  payload and requests a high-priority send through Scheduler and Worker.
- Omitted signals retain the current payload value.

Generate with base overlay first and preset overlay second.
