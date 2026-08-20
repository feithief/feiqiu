/**
 *****************************************************************************
 * @brief   adc_reg_def header file.
 *
 * @file    adc_reg_def.h
 * @author  AE/FAE team
 * @date    2024.09.01
 *****************************************************************************
 *
 * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 * TIME. AS A RESULT, TINYCHIP SHALL NOT BE HELD LIABLE FOR ANY
 * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
 * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *
 * <b>&copy; Copyright (c) 2024 Tinychip Microelectronics Co.,Ltd.</b>
 *
 *****************************************************************************
 */

#ifndef __ADC_DEF_H__
#define __ADC_DEF_H__

// Register Offset Macro Definition
#define ADC_FIFO_DATA_OFFSET                                    0x0000
#define ADC_FIFO_STATE_OFFSET                                   0x0004
#define ADC_IMR_OFFSET                                          0x0008
#define ADC_ICR_OFFSET                                          0x000C
#define ADC_ISR_OFFSET                                          0x0010
#define ADC_IRSR_OFFSET                                         0x0014
#define ADC_CTRL0_OFFSET                                        0x0018
#define ADC_CTRL1_OFFSET                                        0x001C
#define ADC_CTRL2_OFFSET                                        0x0020
#define ADC_CTRL3_OFFSET                                        0x0024
#define ADC_CTRL_SCAN01_OFFSET                                  0x0028
#define ADC_CTRL_SCAN23_OFFSET                                  0x002C
#define ADC_CTRL_SCAN45_OFFSET                                  0x0030
#define ADC_CTRL_SCAN67_OFFSET                                  0x0034
#define ADC_ADC_OUT_OFFSET                                      0x0038
#define ADC_CHNL_STATUS_OFFSET                                  0x003C
#define ADC_CTRL_ANA_OFFSET                                     0x0040

// Register Address Macro Definition
#define ADC_FIFO_DATA_ADDR                                      ( ADC_BASE_ADDR + ADC_FIFO_DATA_OFFSET)
#define ADC_FIFO_STATE_ADDR                                     ( ADC_BASE_ADDR + ADC_FIFO_STATE_OFFSET)
#define ADC_IMR_ADDR                                            ( ADC_BASE_ADDR + ADC_IMR_OFFSET)
#define ADC_ICR_ADDR                                            ( ADC_BASE_ADDR + ADC_ICR_OFFSET)
#define ADC_ISR_ADDR                                            ( ADC_BASE_ADDR + ADC_ISR_OFFSET)
#define ADC_IRSR_ADDR                                           ( ADC_BASE_ADDR + ADC_IRSR_OFFSET)
#define ADC_CTRL0_ADDR                                          ( ADC_BASE_ADDR + ADC_CTRL0_OFFSET)
#define ADC_CTRL1_ADDR                                          ( ADC_BASE_ADDR + ADC_CTRL1_OFFSET)
#define ADC_CTRL2_ADDR                                          ( ADC_BASE_ADDR + ADC_CTRL2_OFFSET)
#define ADC_CTRL3_ADDR                                          ( ADC_BASE_ADDR + ADC_CTRL3_OFFSET)
#define ADC_CTRL_SCAN01_ADDR                                    ( ADC_BASE_ADDR + ADC_CTRL_SCAN01_OFFSET)
#define ADC_CTRL_SCAN23_ADDR                                    ( ADC_BASE_ADDR + ADC_CTRL_SCAN23_OFFSET)
#define ADC_CTRL_SCAN45_ADDR                                    ( ADC_BASE_ADDR + ADC_CTRL_SCAN45_OFFSET)
#define ADC_CTRL_SCAN67_ADDR                                    ( ADC_BASE_ADDR + ADC_CTRL_SCAN67_OFFSET)
#define ADC_ADC_OUT_ADDR                                        ( ADC_BASE_ADDR + ADC_ADC_OUT_OFFSET)
#define ADC_CHNL_STATUS_ADDR                                    ( ADC_BASE_ADDR + ADC_CHNL_STATUS_OFFSET)
#define ADC_CTRL_ANA_ADDR                                       ( ADC_BASE_ADDR + ADC_CTRL_ANA_OFFSET)

// Register Field Macro Definition
#define ADC_FIFO_DATA_FIFO_RDATA_SHIFT                          0
#define ADC_FIFO_DATA_FIFO_RDATA_MASK                           0xFFFFFFFF
#define ADC_FIFO_DATA_FIFO_RDATA_SET(n)                         (((uint32_t)(n) << 0  ) & 0xFFFFFFFF)
#define ADC_FIFO_DATA_FIFO_RDATA_CLR                            0x00000000

#define ADC_FIFO_STATE_ENTRY_VALID_SHIFT                        0
#define ADC_FIFO_STATE_ENTRY_VALID_MASK                         0x0000000F
#define ADC_FIFO_STATE_ENTRY_VALID_SET(n)                       (((uint32_t)(n) << 0  ) & 0x0000000F)
#define ADC_FIFO_STATE_ENTRY_VALID_CLR                          0xFFFFFFF0

#define ADC_IMR_IMR_SHIFT                                       0
#define ADC_IMR_IMR_MASK                                        0x0000001F
#define ADC_IMR_IMR_SET(n)                                      (((uint32_t)(n) << 0  ) & 0x0000001F)
#define ADC_IMR_IMR_CLR                                         0xFFFFFFE0

#define ADC_ICR_ICR_SHIFT                                       0
#define ADC_ICR_ICR_MASK                                        0x0000001F
#define ADC_ICR_ICR_SET(n)                                      (((uint32_t)(n) << 0  ) & 0x0000001F)
#define ADC_ICR_ICR_CLR                                         0xFFFFFFE0

#define ADC_ISR_ISR_SHIFT                                       0
#define ADC_ISR_ISR_MASK                                        0x0000001F
#define ADC_ISR_ISR_SET(n)                                      (((uint32_t)(n) << 0  ) & 0x0000001F)
#define ADC_ISR_ISR_CLR                                         0xFFFFFFE0

#define ADC_IRSR_IRSR_SHIFT                                     0
#define ADC_IRSR_IRSR_MASK                                      0x0000001F
#define ADC_IRSR_IRSR_SET(n)                                    (((uint32_t)(n) << 0  ) & 0x0000001F)
#define ADC_IRSR_IRSR_CLR                                       0xFFFFFFE0

#define ADC_CTRL0_VCR_SEL_SHIFT                                 29
#define ADC_CTRL0_VCR_SEL_MASK                                  0xE0000000
#define ADC_CTRL0_VCR_SEL_SET(n)                                (((uint32_t)(n) << 29 ) & 0xE0000000)
#define ADC_CTRL0_VCR_SEL_CLR                                   0x1FFFFFFF

#define ADC_CTRL0_VCR_EN_SHIFT                                  28
#define ADC_CTRL0_VCR_EN_MASK                                   0x10000000
#define ADC_CTRL0_VCR_EN_SET(n)                                 (((uint32_t)(n) << 28 ) & 0x10000000)
#define ADC_CTRL0_VCR_EN_CLR                                    0xEFFFFFFF

#define ADC_CTRL0_TRIG_EN_SHIFT                                 16
#define ADC_CTRL0_TRIG_EN_MASK                                  0x03FF0000
#define ADC_CTRL0_TRIG_EN_SET(n)                                (((uint32_t)(n) << 16 ) & 0x03FF0000)
#define ADC_CTRL0_TRIG_EN_CLR                                   0xFC00FFFF

#define ADC_CTRL0_FIFO_THRHLD_SHIFT                             12
#define ADC_CTRL0_FIFO_THRHLD_MASK                              0x00007000
#define ADC_CTRL0_FIFO_THRHLD_SET(n)                            (((uint32_t)(n) << 12 ) & 0x00007000)
#define ADC_CTRL0_FIFO_THRHLD_CLR                               0xFFFF8FFF

#define ADC_CTRL0_IRQ_MODE_SHIFT                                8
#define ADC_CTRL0_IRQ_MODE_MASK                                 0x00000700
#define ADC_CTRL0_IRQ_MODE_SET(n)                               (((uint32_t)(n) << 8  ) & 0x00000700)
#define ADC_CTRL0_IRQ_MODE_CLR                                  0xFFFFF8FF

#define ADC_CTRL0_VCM_SEL_SHIFT                                 6
#define ADC_CTRL0_VCM_SEL_MASK                                  0x000000C0
#define ADC_CTRL0_VCM_SEL_SET(n)                                (((uint32_t)(n) << 6  ) & 0x000000C0)
#define ADC_CTRL0_VCM_SEL_CLR                                   0xFFFFFF3F

#define ADC_CTRL0_VREFBUF_EN_SHIFT                              5
#define ADC_CTRL0_VREFBUF_EN_MASK                               0x00000020
#define ADC_CTRL0_VREFBUF_EN_SET(n)                             (((uint32_t)(n) << 5  ) & 0x00000020)
#define ADC_CTRL0_VREFBUF_EN_CLR                                0xFFFFFFDF

#define ADC_CTRL0_I_SEL_SHIFT                                   3
#define ADC_CTRL0_I_SEL_MASK                                    0x00000008
#define ADC_CTRL0_I_SEL_SET(n)                                  (((uint32_t)(n) << 3  ) & 0x00000008)
#define ADC_CTRL0_I_SEL_CLR                                     0xFFFFFFF7

#define ADC_CTRL0_AUTO_ADC_EN_SHIFT                             2
#define ADC_CTRL0_AUTO_ADC_EN_MASK                              0x00000004
#define ADC_CTRL0_AUTO_ADC_EN_SET(n)                            (((uint32_t)(n) << 2  ) & 0x00000004)
#define ADC_CTRL0_AUTO_ADC_EN_CLR                               0xFFFFFFFB

#define ADC_CTRL0_SW_ADC_EN_SHIFT                               1
#define ADC_CTRL0_SW_ADC_EN_MASK                                0x00000002
#define ADC_CTRL0_SW_ADC_EN_SET(n)                              (((uint32_t)(n) << 1  ) & 0x00000002)
#define ADC_CTRL0_SW_ADC_EN_CLR                                 0xFFFFFFFD

#define ADC_CTRL0_SW_START_SHIFT                                0
#define ADC_CTRL0_SW_START_MASK                                 0x00000001
#define ADC_CTRL0_SW_START_SET(n)                               (((uint32_t)(n) << 0  ) & 0x00000001)
#define ADC_CTRL0_SW_START_CLR                                  0xFFFFFFFE

#define ADC_CTRL1_ADC_RESERVE_SHIFT                             30
#define ADC_CTRL1_ADC_RESERVE_MASK                              0xC0000000
#define ADC_CTRL1_ADC_RESERVE_SET(n)                            (((uint32_t)(n) << 30 ) & 0xC0000000)
#define ADC_CTRL1_ADC_RESERVE_CLR                               0x3FFFFFFF

#define ADC_CTRL1_INT1_SNS_EN_SHIFT                             29
#define ADC_CTRL1_INT1_SNS_EN_MASK                              0x20000000
#define ADC_CTRL1_INT1_SNS_EN_SET(n)                            (((uint32_t)(n) << 29 ) & 0x20000000)
#define ADC_CTRL1_INT1_SNS_EN_CLR                               0xDFFFFFFF

#define ADC_CTRL1_INT0_SNS_EN_SHIFT                             28
#define ADC_CTRL1_INT0_SNS_EN_MASK                              0x10000000
#define ADC_CTRL1_INT0_SNS_EN_SET(n)                            (((uint32_t)(n) << 28 ) & 0x10000000)
#define ADC_CTRL1_INT0_SNS_EN_CLR                               0xEFFFFFFF

#define ADC_CTRL1_BUFP_EN_SHIFT                                 27
#define ADC_CTRL1_BUFP_EN_MASK                                  0x08000000
#define ADC_CTRL1_BUFP_EN_SET(n)                                (((uint32_t)(n) << 27 ) & 0x08000000)
#define ADC_CTRL1_BUFP_EN_CLR                                   0xF7FFFFFF

#define ADC_CTRL1_BUFN_EN_SHIFT                                 26
#define ADC_CTRL1_BUFN_EN_MASK                                  0x04000000
#define ADC_CTRL1_BUFN_EN_SET(n)                                (((uint32_t)(n) << 26 ) & 0x04000000)
#define ADC_CTRL1_BUFN_EN_CLR                                   0xFBFFFFFF

#define ADC_CTRL1_PGA_EN_SHIFT                                  25
#define ADC_CTRL1_PGA_EN_MASK                                   0x02000000
#define ADC_CTRL1_PGA_EN_SET(n)                                 (((uint32_t)(n) << 25 ) & 0x02000000)
#define ADC_CTRL1_PGA_EN_CLR                                    0xFDFFFFFF

#define ADC_CTRL1_DIV_EN_SHIFT                                  24
#define ADC_CTRL1_DIV_EN_MASK                                   0x01000000
#define ADC_CTRL1_DIV_EN_SET(n)                                 (((uint32_t)(n) << 24 ) & 0x01000000)
#define ADC_CTRL1_DIV_EN_CLR                                    0xFEFFFFFF

#define ADC_CTRL1_SW_CONT_MODE_SHIFT                            23
#define ADC_CTRL1_SW_CONT_MODE_MASK                             0x00800000
#define ADC_CTRL1_SW_CONT_MODE_SET(n)                           (((uint32_t)(n) << 23 ) & 0x00800000)
#define ADC_CTRL1_SW_CONT_MODE_CLR                              0xFF7FFFFF

#define ADC_CTRL1_CHNL_SAMP_NUM_SHIFT                           20
#define ADC_CTRL1_CHNL_SAMP_NUM_MASK                            0x00700000
#define ADC_CTRL1_CHNL_SAMP_NUM_SET(n)                          (((uint32_t)(n) << 20 ) & 0x00700000)
#define ADC_CTRL1_CHNL_SAMP_NUM_CLR                             0xFF8FFFFF

#define ADC_CTRL1_SCAN_CHNL_NUM_SHIFT                           16
#define ADC_CTRL1_SCAN_CHNL_NUM_MASK                            0x00070000
#define ADC_CTRL1_SCAN_CHNL_NUM_SET(n)                          (((uint32_t)(n) << 16 ) & 0x00070000)
#define ADC_CTRL1_SCAN_CHNL_NUM_CLR                             0xFFF8FFFF

#define ADC_CTRL1_VREF_SEL_SHIFT                                14
#define ADC_CTRL1_VREF_SEL_MASK                                 0x0000C000
#define ADC_CTRL1_VREF_SEL_SET(n)                               (((uint32_t)(n) << 14 ) & 0x0000C000)
#define ADC_CTRL1_VREF_SEL_CLR                                  0xFFFF3FFF

#define ADC_CTRL1_PGA_GAIN_SEL_SHIFT                            8
#define ADC_CTRL1_PGA_GAIN_SEL_MASK                             0x00000F00
#define ADC_CTRL1_PGA_GAIN_SEL_SET(n)                           (((uint32_t)(n) << 8  ) & 0x00000F00)
#define ADC_CTRL1_PGA_GAIN_SEL_CLR                              0xFFFFF0FF

#define ADC_CTRL1_BUFP_BP_SHIFT                                 7
#define ADC_CTRL1_BUFP_BP_MASK                                  0x00000080
#define ADC_CTRL1_BUFP_BP_SET(n)                                (((uint32_t)(n) << 7  ) & 0x00000080)
#define ADC_CTRL1_BUFP_BP_CLR                                   0xFFFFFF7F

#define ADC_CTRL1_BUFN_BP_SHIFT                                 6
#define ADC_CTRL1_BUFN_BP_MASK                                  0x00000040
#define ADC_CTRL1_BUFN_BP_SET(n)                                (((uint32_t)(n) << 6  ) & 0x00000040)
#define ADC_CTRL1_BUFN_BP_CLR                                   0xFFFFFFBF

#define ADC_CTRL1_PGA_BP_SHIFT                                  5
#define ADC_CTRL1_PGA_BP_MASK                                   0x00000020
#define ADC_CTRL1_PGA_BP_SET(n)                                 (((uint32_t)(n) << 5  ) & 0x00000020)
#define ADC_CTRL1_PGA_BP_CLR                                    0xFFFFFFDF

#define ADC_CTRL1_IN_SEL_SHIFT                                  0
#define ADC_CTRL1_IN_SEL_MASK                                   0x0000001F
#define ADC_CTRL1_IN_SEL_SET(n)                                 (((uint32_t)(n) << 0  ) & 0x0000001F)
#define ADC_CTRL1_IN_SEL_CLR                                    0xFFFFFFE0

#define ADC_CTRL2_SAMP_CYCLE_SHIFT                              16
#define ADC_CTRL2_SAMP_CYCLE_MASK                               0x007F0000
#define ADC_CTRL2_SAMP_CYCLE_SET(n)                             (((uint32_t)(n) << 16 ) & 0x007F0000)
#define ADC_CTRL2_SAMP_CYCLE_CLR                                0xFF80FFFF

#define ADC_CTRL2_INIT_CYCLE_SHIFT                              0
#define ADC_CTRL2_INIT_CYCLE_MASK                               0x00000FFF
#define ADC_CTRL2_INIT_CYCLE_SET(n)                             (((uint32_t)(n) << 0  ) & 0x00000FFF)
#define ADC_CTRL2_INIT_CYCLE_CLR                                0xFFFFF000

#define ADC_CTRL3_CMP_CEILING_SHIFT                             16
#define ADC_CTRL3_CMP_CEILING_MASK                              0x3FFF0000
#define ADC_CTRL3_CMP_CEILING_SET(n)                            (((uint32_t)(n) << 16 ) & 0x3FFF0000)
#define ADC_CTRL3_CMP_CEILING_CLR                               0xC000FFFF

#define ADC_CTRL3_CMP_BOTTOM_SHIFT                              0
#define ADC_CTRL3_CMP_BOTTOM_MASK                               0x00003FFF
#define ADC_CTRL3_CMP_BOTTOM_SET(n)                             (((uint32_t)(n) << 0  ) & 0x00003FFF)
#define ADC_CTRL3_CMP_BOTTOM_CLR                                0xFFFFC000

#define ADC_CTRL_SCAN01_SCAN_CHNL_1_SHIFT                       16
#define ADC_CTRL_SCAN01_SCAN_CHNL_1_MASK                        0x0FFF0000
#define ADC_CTRL_SCAN01_SCAN_CHNL_1_SET(n)                      (((uint32_t)(n) << 16 ) & 0x0FFF0000)
#define ADC_CTRL_SCAN01_SCAN_CHNL_1_CLR                         0xF000FFFF

#define ADC_CTRL_SCAN01_SCAN_CHNL_0_SHIFT                       0
#define ADC_CTRL_SCAN01_SCAN_CHNL_0_MASK                        0x00000FFF
#define ADC_CTRL_SCAN01_SCAN_CHNL_0_SET(n)                      (((uint32_t)(n) << 0  ) & 0x00000FFF)
#define ADC_CTRL_SCAN01_SCAN_CHNL_0_CLR                         0xFFFFF000

#define ADC_CTRL_SCAN23_SCAN_CHNL_3_SHIFT                       16
#define ADC_CTRL_SCAN23_SCAN_CHNL_3_MASK                        0x0FFF0000
#define ADC_CTRL_SCAN23_SCAN_CHNL_3_SET(n)                      (((uint32_t)(n) << 16 ) & 0x0FFF0000)
#define ADC_CTRL_SCAN23_SCAN_CHNL_3_CLR                         0xF000FFFF

#define ADC_CTRL_SCAN23_SCAN_CHNL_2_SHIFT                       0
#define ADC_CTRL_SCAN23_SCAN_CHNL_2_MASK                        0x00000FFF
#define ADC_CTRL_SCAN23_SCAN_CHNL_2_SET(n)                      (((uint32_t)(n) << 0  ) & 0x00000FFF)
#define ADC_CTRL_SCAN23_SCAN_CHNL_2_CLR                         0xFFFFF000

#define ADC_CTRL_SCAN45_SCAN_CHNL_5_SHIFT                       16
#define ADC_CTRL_SCAN45_SCAN_CHNL_5_MASK                        0x0FFF0000
#define ADC_CTRL_SCAN45_SCAN_CHNL_5_SET(n)                      (((uint32_t)(n) << 16 ) & 0x0FFF0000)
#define ADC_CTRL_SCAN45_SCAN_CHNL_5_CLR                         0xF000FFFF

#define ADC_CTRL_SCAN45_SCAN_CHNL_4_SHIFT                       0
#define ADC_CTRL_SCAN45_SCAN_CHNL_4_MASK                        0x00000FFF
#define ADC_CTRL_SCAN45_SCAN_CHNL_4_SET(n)                      (((uint32_t)(n) << 0  ) & 0x00000FFF)
#define ADC_CTRL_SCAN45_SCAN_CHNL_4_CLR                         0xFFFFF000

#define ADC_CTRL_SCAN67_SCAN_CHNL_7_SHIFT                       16
#define ADC_CTRL_SCAN67_SCAN_CHNL_7_MASK                        0x0FFF0000
#define ADC_CTRL_SCAN67_SCAN_CHNL_7_SET(n)                      (((uint32_t)(n) << 16 ) & 0x0FFF0000)
#define ADC_CTRL_SCAN67_SCAN_CHNL_7_CLR                         0xF000FFFF

#define ADC_CTRL_SCAN67_SCAN_CHNL_6_SHIFT                       0
#define ADC_CTRL_SCAN67_SCAN_CHNL_6_MASK                        0x00000FFF
#define ADC_CTRL_SCAN67_SCAN_CHNL_6_SET(n)                      (((uint32_t)(n) << 0  ) & 0x00000FFF)
#define ADC_CTRL_SCAN67_SCAN_CHNL_6_CLR                         0xFFFFF000

#define ADC_ADC_OUT_RAW_OUT17_SHIFT                             15
#define ADC_ADC_OUT_RAW_OUT17_MASK                              0xFFFF8000
#define ADC_ADC_OUT_RAW_OUT17_SET(n)                            (((uint32_t)(n) << 15 ) & 0xFFFF8000)
#define ADC_ADC_OUT_RAW_OUT17_CLR                               0x00007FFF

#define ADC_ADC_OUT_ADC_READY_ERR_SHIFT                         14
#define ADC_ADC_OUT_ADC_READY_ERR_MASK                          0x00004000
#define ADC_ADC_OUT_ADC_READY_ERR_SET(n)                        (((uint32_t)(n) << 14 ) & 0x00004000)
#define ADC_ADC_OUT_ADC_READY_ERR_CLR                           0xFFFFBFFF

#define ADC_ADC_OUT_ADC_OUT_SHIFT                               0
#define ADC_ADC_OUT_ADC_OUT_MASK                                0x00003FFF
#define ADC_ADC_OUT_ADC_OUT_SET(n)                              (((uint32_t)(n) << 0  ) & 0x00003FFF)
#define ADC_ADC_OUT_ADC_OUT_CLR                                 0xFFFFC000

#define ADC_CHNL_STATUS_IRQ_COMP_STATUS_SHIFT                   1
#define ADC_CHNL_STATUS_IRQ_COMP_STATUS_MASK                    0x000003FE
#define ADC_CHNL_STATUS_IRQ_COMP_STATUS_SET(n)                  (((uint32_t)(n) << 1  ) & 0x000003FE)
#define ADC_CHNL_STATUS_IRQ_COMP_STATUS_CLR                     0xFFFFFC01

#define ADC_CHNL_STATUS_ADC_BUSY_SHIFT                          0
#define ADC_CHNL_STATUS_ADC_BUSY_MASK                           0x00000001
#define ADC_CHNL_STATUS_ADC_BUSY_SET(n)                         (((uint32_t)(n) << 0  ) & 0x00000001)
#define ADC_CHNL_STATUS_ADC_BUSY_CLR                            0xFFFFFFFE

#define ADC_CTRL_ANA_ADC_CTRL_SHIFT                             0
#define ADC_CTRL_ANA_ADC_CTRL_MASK                              0x0000FFFF
#define ADC_CTRL_ANA_ADC_CTRL_SET(n)                            (((uint32_t)(n) << 0  ) & 0x0000FFFF)
#define ADC_CTRL_ANA_ADC_CTRL_CLR                               0xFFFF0000

// Register Structure Definition
typedef struct
{
    __IO uint32_t FIFO_RDATA      : 32 ; // 31 : 0
} ADC_FIFO_DATA_FIELD_T;

typedef struct
{
    __IO uint32_t ENTRY_VALID     : 4  ; // 3  : 0
    __IO uint32_t RESERVED0       : 28 ; // 31 : 4
} ADC_FIFO_STATE_FIELD_T;

typedef struct
{
    __IO uint32_t IMR             : 5  ; // 4  : 0
    __IO uint32_t RESERVED0       : 27 ; // 31 : 5
} ADC_IMR_FIELD_T;

typedef struct
{
    __IO uint32_t ICR             : 5  ; // 4  : 0
    __IO uint32_t RESERVED0       : 27 ; // 31 : 5
} ADC_ICR_FIELD_T;

typedef struct
{
    __IO uint32_t ISR             : 5  ; // 4  : 0
    __IO uint32_t RESERVED0       : 27 ; // 31 : 5
} ADC_ISR_FIELD_T;

typedef struct
{
    __IO uint32_t IRSR            : 5  ; // 4  : 0
    __IO uint32_t RESERVED0       : 27 ; // 31 : 5
} ADC_IRSR_FIELD_T;

typedef struct
{
    __IO uint32_t SW_START        : 1  ; // 0  : 0
    __IO uint32_t SW_ADC_EN       : 1  ; // 1  : 1
    __IO uint32_t AUTO_ADC_EN     : 1  ; // 2  : 2
    __IO uint32_t I_SEL           : 1  ; // 3  : 3
    __IO uint32_t RESERVED3       : 1  ; // 4  : 4
    __IO uint32_t VREFBUF_EN      : 1  ; // 5  : 5
    __IO uint32_t VCM_SEL         : 2  ; // 7  : 6
    __IO uint32_t IRQ_MODE        : 3  ; // 10 : 8
    __IO uint32_t RESERVED2       : 1  ; // 11 : 11
    __IO uint32_t FIFO_THRHLD     : 3  ; // 14 : 12
    __IO uint32_t RESERVED1       : 1  ; // 15 : 15
    __IO uint32_t TRIG_EN         : 10 ; // 25 : 16
    __IO uint32_t RESERVED0       : 2  ; // 27 : 26
    __IO uint32_t VCR_EN          : 1  ; // 28 : 28
    __IO uint32_t VCR_SEL         : 3  ; // 31 : 29
} ADC_CTRL0_FIELD_T;

typedef struct
{
    __IO uint32_t IN_SEL          : 5  ; // 4  : 0
    __IO uint32_t PGA_BP          : 1  ; // 5  : 5
    __IO uint32_t BUFN_BP         : 1  ; // 6  : 6
    __IO uint32_t BUFP_BP         : 1  ; // 7  : 7
    __IO uint32_t PGA_GAIN_SEL    : 4  ; // 11 : 8
    __IO uint32_t RESERVED1       : 2  ; // 13 : 12
    __IO uint32_t VREF_SEL        : 2  ; // 15 : 14
    __IO uint32_t SCAN_CHNL_NUM   : 3  ; // 18 : 16
    __IO uint32_t RESERVED0       : 1  ; // 19 : 19
    __IO uint32_t CHNL_SAMP_NUM   : 3  ; // 22 : 20
    __IO uint32_t SW_CONT_MODE    : 1  ; // 23 : 23
    __IO uint32_t DIV_EN          : 1  ; // 24 : 24
    __IO uint32_t PGA_EN          : 1  ; // 25 : 25
    __IO uint32_t BUFN_EN         : 1  ; // 26 : 26
    __IO uint32_t BUFP_EN         : 1  ; // 27 : 27
    __IO uint32_t INT0_SNS_EN     : 1  ; // 28 : 28
    __IO uint32_t INT1_SNS_EN     : 1  ; // 29 : 29
    __IO uint32_t ADC_RESERVE     : 2  ; // 31 : 30
} ADC_CTRL1_FIELD_T;

typedef struct
{
    __IO uint32_t INIT_CYCLE      : 12 ; // 11 : 0
    __IO uint32_t RESERVED1       : 4  ; // 15 : 12
    __IO uint32_t SAMP_CYCLE      : 7  ; // 22 : 16
    __IO uint32_t RESERVED0       : 9  ; // 31 : 23
} ADC_CTRL2_FIELD_T;

typedef struct
{
    __IO uint32_t CMP_BOTTOM      : 14 ; // 13 : 0
    __IO uint32_t RESERVED1       : 2  ; // 15 : 14
    __IO uint32_t CMP_CEILING     : 14 ; // 29 : 16
    __IO uint32_t RESERVED0       : 2  ; // 31 : 30
} ADC_CTRL3_FIELD_T;

typedef struct
{
    __IO uint32_t SCAN_CHNL_0     : 12 ; // 11 : 0
    __IO uint32_t RESERVED1       : 4  ; // 15 : 12
    __IO uint32_t SCAN_CHNL_1     : 12 ; // 27 : 16
    __IO uint32_t RESERVED0       : 4  ; // 31 : 28
} ADC_CTRL_SCAN01_FIELD_T;

typedef struct
{
    __IO uint32_t SCAN_CHNL_2     : 12 ; // 11 : 0
    __IO uint32_t RESERVED1       : 4  ; // 15 : 12
    __IO uint32_t SCAN_CHNL_3     : 12 ; // 27 : 16
    __IO uint32_t RESERVED0       : 4  ; // 31 : 28
} ADC_CTRL_SCAN23_FIELD_T;

typedef struct
{
    __IO uint32_t SCAN_CHNL_4     : 12 ; // 11 : 0
    __IO uint32_t RESERVED1       : 4  ; // 15 : 12
    __IO uint32_t SCAN_CHNL_5     : 12 ; // 27 : 16
    __IO uint32_t RESERVED0       : 4  ; // 31 : 28
} ADC_CTRL_SCAN45_FIELD_T;

typedef struct
{
    __IO uint32_t SCAN_CHNL_6     : 12 ; // 11 : 0
    __IO uint32_t RESERVED1       : 4  ; // 15 : 12
    __IO uint32_t SCAN_CHNL_7     : 12 ; // 27 : 16
    __IO uint32_t RESERVED0       : 4  ; // 31 : 28
} ADC_CTRL_SCAN67_FIELD_T;

typedef struct
{
    __IO uint32_t ADC_OUT         : 14 ; // 13 : 0
    __IO uint32_t ADC_READY_ERR   : 1  ; // 14 : 14
    __IO uint32_t RAW_OUT17       : 17 ; // 31 : 15
} ADC_ADC_OUT_FIELD_T;

typedef struct
{
    __IO uint32_t ADC_BUSY        : 1  ; // 0  : 0
    __IO uint32_t IRQ_COMP_STATUS : 9  ; // 9  : 1
    __IO uint32_t RESERVED0       : 22 ; // 31 : 10
} ADC_CHNL_STATUS_FIELD_T;

typedef struct
{
    __IO uint32_t ADC_CTRL        : 16 ; // 15 : 0
    __IO uint32_t RESERVED0       : 16 ; // 31 : 16
} ADC_CTRL_ANA_FIELD_T;

// Register Map Structure Definition
typedef struct
{
    union
    {
        __IO  uint32_t FIFO_DATA                        ; // 0x0000
        __IO  ADC_FIFO_DATA_FIELD_T FIFO_DATA_F         ;
    };

    union
    {
        __IO  uint32_t FIFO_STATE                       ; // 0x0004
        __IO  ADC_FIFO_STATE_FIELD_T FIFO_STATE_F       ;
    };

    union
    {
        __IO  uint32_t IMR                              ; // 0x0008
        __IO  ADC_IMR_FIELD_T IMR_F                     ;
    };

    union
    {
        __IO  uint32_t ICR                              ; // 0x000C
        __IO  ADC_ICR_FIELD_T ICR_F                     ;
    };

    union
    {
        __IO  uint32_t ISR                              ; // 0x0010
        __IO  ADC_ISR_FIELD_T ISR_F                     ;
    };

    union
    {
        __IO  uint32_t IRSR                             ; // 0x0014
        __IO  ADC_IRSR_FIELD_T IRSR_F                   ;
    };

    union
    {
        __IO  uint32_t CTRL0                            ; // 0x0018
        __IO  ADC_CTRL0_FIELD_T CTRL0_F                 ;
    };

    union
    {
        __IO  uint32_t CTRL1                            ; // 0x001C
        __IO  ADC_CTRL1_FIELD_T CTRL1_F                 ;
    };

    union
    {
        __IO  uint32_t CTRL2                            ; // 0x0020
        __IO  ADC_CTRL2_FIELD_T CTRL2_F                 ;
    };

    union
    {
        __IO  uint32_t CTRL3                            ; // 0x0024
        __IO  ADC_CTRL3_FIELD_T CTRL3_F                 ;
    };

    union
    {
        __IO  uint32_t CTRL_SCAN01                      ; // 0x0028
        __IO  ADC_CTRL_SCAN01_FIELD_T CTRL_SCAN01_F     ;
    };

    union
    {
        __IO  uint32_t CTRL_SCAN23                      ; // 0x002C
        __IO  ADC_CTRL_SCAN23_FIELD_T CTRL_SCAN23_F     ;
    };

    union
    {
        __IO  uint32_t CTRL_SCAN45                      ; // 0x0030
        __IO  ADC_CTRL_SCAN45_FIELD_T CTRL_SCAN45_F     ;
    };

    union
    {
        __IO  uint32_t CTRL_SCAN67                      ; // 0x0034
        __IO  ADC_CTRL_SCAN67_FIELD_T CTRL_SCAN67_F     ;
    };

    union
    {
        __IO  uint32_t ADC_OUT                          ; // 0x0038
        __IO  ADC_ADC_OUT_FIELD_T ADC_OUT_F             ;
    };

    union
    {
        __IO  uint32_t CHNL_STATUS                      ; // 0x003C
        __IO  ADC_CHNL_STATUS_FIELD_T CHNL_STATUS_F     ;
    };

    union
    {
        __IO  uint32_t CTRL_ANA                         ; // 0x0040
        __IO  ADC_CTRL_ANA_FIELD_T CTRL_ANA_F           ;
    };

} ADC_REG_TypeDef;

#endif /* __ADC_DEF_H__ */
