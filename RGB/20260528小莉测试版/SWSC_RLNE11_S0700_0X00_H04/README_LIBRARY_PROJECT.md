# Sealed Library Project

This directory is generated from `21us/SWSC_RLNE11_S0700_0X00_H04`.

## Projects

- `SWSC_Application.uvprojx` compiles only RTE sources plus external configuration, then links `SWSC_Core.lib`.
- `build_all.cmd` performs the same one-click rebuild from the command line.

## Sealed Sources

`Library/Objects/SWSC_Core.lib` contains the compiled code from:

- `System`
- `Protocol`
- `modules`
- `Hal`
- `Generated`
- `dimming`
- `App`
- `Algorithm`
- `AA`

RTE sources are excluded from the library and remain in the application project.

The delivery folder does not contain the C source files from the sealed directories.
Only public headers are retained for RTE compilation.

## External Configuration

- Edit `Config/lin_autoaddressing_cfg.h` for AA timing, T2 values, ADC restore values, complete AA phase register values, and thresholds.
- `LIN_AA_USE_DELAYED_STEP_TIMING` still selects the default step table, and the `D_STEPx_*_BIT` values inside either branch can also be edited directly.
- These values are compiled into `Config/lin_autoaddressing_cfg.c` outside the sealed library and are read by the library at runtime.
- Edit the threshold initial values in `Config/lin_autoaddressing_cfg.h`; their storage is provided by `Config/lin_autoaddressing_cfg.c` outside the library.
- `PublicInclude` contains the public headers required to compile RTE without depending on sealed source directories.
- No sealed-library source builder is included in the delivery.

## One-click Use

1. Edit `Config/lin_autoaddressing_cfg.h`.
2. Open `SWSC_Application.uvprojx`.
3. Press Rebuild.
4. Use `Objects/SWSC_Application.hex`.

Only exposed runtime parameters can change without source code.
Arbitrary edits to sealed implementation headers cannot affect a precompiled library.

## Verified Result

The application builds with Keil ARM Compiler 6.21 with zero errors and zero warnings.
The default external configuration preserves the original AA register values and behavior, but the HEX differs because the sealed library now reads runtime configuration data.
Changing `Config/lin_autoaddressing_cfg.h` and rebuilding changes the application HEX without rebuilding or exposing `SWSC_Core.lib` source.
