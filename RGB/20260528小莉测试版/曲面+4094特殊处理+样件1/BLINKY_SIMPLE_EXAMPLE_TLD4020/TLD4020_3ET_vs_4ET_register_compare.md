# TLD4020-3ET vs TLD4020-4ET Register Compare

Sources:
- 3ET SVD: `SWSC_RLNE11_S0700_0X00_H04\RTE\Device\TLD4020-3ET\tld40xx.svd.update@4.1.2`
- 4ET SVD: `BLINKY_SIMPLE_EXAMPLE_TLD4020\RTE\Device\TLD4020-4ET\tld40xx_4et.svd`
- Full register CSV: `TLD4020_3ET_vs_4ET_register_compare.csv`

## Peripheral Summary

| Peripheral | 3ET base | 4ET base | 3ET regs | 4ET regs | Result |
|---|---:|---:|---:|---:|---|
| PMU | 0x40000000 | 0x40000000 | 33 | 33 | same register names/offsets/fields |
| SCU | 0x40004000 | 0x40004000 | 10 | 10 | same register names/offsets/fields |
| MEM | 0x40008000 | 0x40008000 | 11 | 11 | same register names/offsets/fields |
| ARM | 0x40010000 | 0x40010000 | 20 | 20 | same register names/offsets/fields |
| OSC | 0x40014000 | 0x40014000 | 1 | 1 | same register names/offsets/fields |
| GPIO | 0x48000000 | 0x48000000 | 14 | 14 | 2 register/field differences |
| LIN | 0x48008000 | 0x48008000 | 23 | 23 | 1 register/field differences |
| ADC11 | 0x4800C000 | 0x4800C000 | 8 | 8 | same register names/offsets/fields |
| LDRV | 0x48014000 | 0x48014000 | 34 | 39 | 33 register/field differences |
| UART2 | - | 0x48020000 | - | 10 | 4ET only |
| CPU | 0xE0000000 | 0xE0000000 | 42 | 42 | same register names/offsets/fields |

## Changed Registers And Fields

### GPIO

| Register | 3ET offset | 4ET offset | Status | Field diff |
|---|---:|---:|---|---|
| PADCFG0_DRVCFG | 0x304 | 0x304 | field changed | +field OD[1:1] |
| PADCFG1_DRVCFG | 0x314 | 0x314 | field changed | +field OD[1:1] |

### LIN

| Register | 3ET offset | 4ET offset | Status | Field diff |
|---|---:|---:|---|---|
| WAKE_CNF | 0x64 | 0x64 | field changed | +field WU_MODE_CFG[3:3]; +field WPUDIS[4:4] |

### LDRV

| Register | 3ET offset | 4ET offset | Status | Field diff |
|---|---:|---:|---|---|
| PWM_DC_CFG0 | 0x18 | 0x1C | offset changed |  |
| PWM_PHASE_CFG3 | - | 0x18 | 4ET only |  |
| PWM_DC_CFG1 | 0x1C | 0x20 | offset changed |  |
| PWM_DC_CFG2 | 0x20 | 0x24 | offset changed |  |
| PWM_ISET_CFG0 | 0x24 | 0x2C | offset changed |  |
| PWM_DC_CFG3 | - | 0x28 | 4ET only |  |
| PWM_ISET_CFG1 | 0x28 | 0x30 | offset changed |  |
| PWM_ISET_CFG2 | 0x2C | 0x34 | offset changed |  |
| OSP_CFG | 0x34 | 0x3C | offset changed |  |
| OSP_IS | 0x38 | 0x40 | offset changed; field changed | field TH_OVLD (0, 3)->(0, 4); field S2G (16, 3)->(16, 4) |
| PWM_ISET_CFG3 | - | 0x38 | 4ET only |  |
| OSP_SC | 0x3C | 0x44 | offset changed; field changed | field TH_OVLD (0, 3)->(0, 4); field S2G (16, 3)->(16, 4) |
| OSP_SS | 0x40 | 0x4C | offset changed; field changed | field TH_OVLD (0, 3)->(0, 4); field S2G (16, 3)->(16, 4) |
| DIAG_CFG | 0x44 | 0x50 | offset changed; field changed | +field PU_EN3[7:7] |
| DIAG_PHASE_CFG0 | 0x48 | 0x54 | offset changed |  |
| DIAG_PHASE_CFG1 | 0x4C | 0x58 | offset changed |  |
| DIAG_PHASE_CFG2 | 0x50 | 0x5C | offset changed |  |
| DIAG_VS | 0x54 | 0x64 | offset changed |  |
| DIAG_VFWD0 | 0x58 | 0x68 | offset changed |  |
| DIAG_VFWD1 | 0x5C | 0x6C | offset changed |  |
| DIAG_PHASE_CFG3 | - | 0x60 | 4ET only |  |
| DIAG_VFWD2 | 0x60 | 0x70 | offset changed |  |
| DIAG_VGPIO0 | 0x64 | 0x78 | offset changed |  |
| DIAG_VGPIO1 | 0x68 | 0x7C | offset changed |  |
| IEN0 | 0x6C | 0x80 | offset changed; field changed | field EOC (16, 3)->(16, 4) |
| IS0 | 0x70 | 0x84 | offset changed; field changed | field EOC (16, 3)->(16, 4) |
| DIAG_VFWD3 | - | 0x74 | 4ET only |  |
| ISC0 | 0x74 | 0x88 | offset changed; field changed | field EOC (16, 3)->(16, 4) |
| ISS0 | 0x78 | 0x8C | offset changed; field changed | field EOC (16, 3)->(16, 4) |
| IEN1 | 0x7C | 0x90 | offset changed; field changed | field TH_OVLD (0, 3)->(0, 4); field S2G (16, 3)->(16, 4) |
| IS1 | 0x80 | 0x94 | offset changed; field changed | field TH_OVLD (0, 3)->(0, 4); field S2G (16, 3)->(16, 4) |
| ISC1 | 0x84 | 0x98 | offset changed; field changed | field TH_OVLD (0, 3)->(0, 4); field S2G (16, 3)->(16, 4) |
| ISS1 | 0x88 | 0x9C | offset changed; field changed | field TH_OVLD (0, 3)->(0, 4); field S2G (16, 3)->(16, 4) |

### UART2

| Register | 3ET offset | 4ET offset | Status | Field diff |
|---|---:|---:|---|---|
| UART_INSEL | - | 0x0 | 4ET only |  |
| UART_SCON | - | 0x4 | 4ET only |  |
| UART_TSTART | - | 0x8 | 4ET only |  |
| UART_TXBUF | - | 0xC | 4ET only |  |
| UART_RXBUF | - | 0x10 | 4ET only |  |
| UART_BCON | - | 0x14 | 4ET only |  |
| UART_IS | - | 0x18 | 4ET only |  |
| UART_ISS | - | 0x1C | 4ET only |  |
| UART_ISC | - | 0x20 | 4ET only |  |
| UART_IEN | - | 0x24 | 4ET only |  |

## ADC Channel Map Used By The Port

| Signal | 3ET ADC channel | 4ET ADC channel | Porting note |
|---|---:|---:|---|
| VS | 0 | 0 | same |
| VFWD LED0 | 1 | 1 | same |
| VFWD LED1 | 2 | 2 | same |
| VFWD LED2 | 3 | 3 | same |
| VFWD LED3 | - | 4 | new 4ET channel |
| GPIO0 | 4 | 6 | shifted by LED3/reserved CH5 |
| GPIO1 | 5 | 7 | shifted by LED3/reserved CH5 |
| LINAA1 | 7 | 9 | shifted |
| LINAA2 | 8 | 10 | shifted |
| Temperature sensor | 9 | 11 | shifted |
