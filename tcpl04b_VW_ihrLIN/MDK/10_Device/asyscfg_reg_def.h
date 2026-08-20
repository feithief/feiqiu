/**
 *****************************************************************************
 * @brief   asyscfg_reg_def header file.
 *
 * @file    asyscfg_reg_def.h
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

#ifndef __ASYSCFG_DEF_H__
#define __ASYSCFG_DEF_H__

// Register Offset Macro Definition
#define ASYSCFG_SLEEP_MODE_OFFSET                               0x0000
#define ASYSCFG_WAIT_CYCLE_OFFSET                               0x0004
#define ASYSCFG_RCCLK_CTRL_OFFSET                               0x0008
#define ASYSCFG_RST_CTRL_OFFSET                                 0x000C
#define ASYSCFG_VS_LVD_CTRL_OFFSET                              0x0010
#define ASYSCFG_LDO33_CTRL_OFFSET                               0x0014
#define ASYSCFG_LDO15_CTRL_OFFSET                               0x0018
#define ASYSCFG_VS_ALT_CTRL_OFFSET                              0x001C
#define ASYSCFG_OPT_STATUS_OFFSET                               0x0020
#define ASYSCFG_PMU_IRQ_CTRL_OFFSET                             0x0024
#define ASYSCFG_IMR_OFFSET                                      0x0028
#define ASYSCFG_ICR_OFFSET                                      0x002C
#define ASYSCFG_ISR_OFFSET                                      0x0030
#define ASYSCFG_IRSR_OFFSET                                     0x0034
#define ASYSCFG_WU_CTRL_OFFSET                                  0x0038
#define ASYSCFG_ADC_CAL_CTRL_OFFSET                             0x003C
#define ASYSCFG_OTP_RST_CTRL_OFFSET                             0x0040
#define ASYSCFG_IO4_RST_CTRL_OFFSET                             0x0044
#define ASYSCFG_RESERVED12_CTRL_OFFSET                          0x0048
#define ASYSCFG_IO4_RST_FILTER_OFFSET                           0x004C
#define ASYSCFG_AON_IO_CTRL_OFFSET                              0x0050
#define ASYSCFG_BKUP0_OFFSET                                    0x00F0
#define ASYSCFG_BKUP1_OFFSET                                    0x00F4
#define ASYSCFG_PATCH_BIT_OFFSET                                0x00F8
#define ASYSCFG_ASYSCFG_LOCK_OFFSET                             0x00FC

// Register Address Macro Definition
#define ASYSCFG_SLEEP_MODE_ADDR                                 ( ASYSCFG_BASE_ADDR + ASYSCFG_SLEEP_MODE_OFFSET)
#define ASYSCFG_WAIT_CYCLE_ADDR                                 ( ASYSCFG_BASE_ADDR + ASYSCFG_WAIT_CYCLE_OFFSET)
#define ASYSCFG_RCCLK_CTRL_ADDR                                 ( ASYSCFG_BASE_ADDR + ASYSCFG_RCCLK_CTRL_OFFSET)
#define ASYSCFG_RST_CTRL_ADDR                                   ( ASYSCFG_BASE_ADDR + ASYSCFG_RST_CTRL_OFFSET)
#define ASYSCFG_VS_LVD_CTRL_ADDR                                ( ASYSCFG_BASE_ADDR + ASYSCFG_VS_LVD_CTRL_OFFSET)
#define ASYSCFG_LDO33_CTRL_ADDR                                 ( ASYSCFG_BASE_ADDR + ASYSCFG_LDO33_CTRL_OFFSET)
#define ASYSCFG_LDO15_CTRL_ADDR                                 ( ASYSCFG_BASE_ADDR + ASYSCFG_LDO15_CTRL_OFFSET)
#define ASYSCFG_VS_ALT_CTRL_ADDR                                ( ASYSCFG_BASE_ADDR + ASYSCFG_VS_ALT_CTRL_OFFSET)
#define ASYSCFG_OPT_STATUS_ADDR                                 ( ASYSCFG_BASE_ADDR + ASYSCFG_OPT_STATUS_OFFSET)
#define ASYSCFG_PMU_IRQ_CTRL_ADDR                               ( ASYSCFG_BASE_ADDR + ASYSCFG_PMU_IRQ_CTRL_OFFSET)
#define ASYSCFG_IMR_ADDR                                        ( ASYSCFG_BASE_ADDR + ASYSCFG_IMR_OFFSET)
#define ASYSCFG_ICR_ADDR                                        ( ASYSCFG_BASE_ADDR + ASYSCFG_ICR_OFFSET)
#define ASYSCFG_ISR_ADDR                                        ( ASYSCFG_BASE_ADDR + ASYSCFG_ISR_OFFSET)
#define ASYSCFG_IRSR_ADDR                                       ( ASYSCFG_BASE_ADDR + ASYSCFG_IRSR_OFFSET)
#define ASYSCFG_WU_CTRL_ADDR                                    ( ASYSCFG_BASE_ADDR + ASYSCFG_WU_CTRL_OFFSET)
#define ASYSCFG_ADC_CAL_CTRL_ADDR                               ( ASYSCFG_BASE_ADDR + ASYSCFG_ADC_CAL_CTRL_OFFSET)
#define ASYSCFG_OTP_RST_CTRL_ADDR                               ( ASYSCFG_BASE_ADDR + ASYSCFG_OTP_RST_CTRL_OFFSET)
#define ASYSCFG_IO4_RST_CTRL_ADDR                               ( ASYSCFG_BASE_ADDR + ASYSCFG_IO4_RST_CTRL_OFFSET)
#define ASYSCFG_RESERVED12_CTRL_ADDR                            ( ASYSCFG_BASE_ADDR + ASYSCFG_RESERVED12_CTRL_OFFSET)
#define ASYSCFG_IO4_RST_FILTER_ADDR                             ( ASYSCFG_BASE_ADDR + ASYSCFG_IO4_RST_FILTER_OFFSET)
#define ASYSCFG_AON_IO_CTRL_ADDR                                ( ASYSCFG_BASE_ADDR + ASYSCFG_AON_IO_CTRL_OFFSET)
#define ASYSCFG_BKUP0_ADDR                                      ( ASYSCFG_BASE_ADDR + ASYSCFG_BKUP0_OFFSET)
#define ASYSCFG_BKUP1_ADDR                                      ( ASYSCFG_BASE_ADDR + ASYSCFG_BKUP1_OFFSET)
#define ASYSCFG_PATCH_BIT_ADDR                                  ( ASYSCFG_BASE_ADDR + ASYSCFG_PATCH_BIT_OFFSET)
#define ASYSCFG_ASYSCFG_LOCK_ADDR                               ( ASYSCFG_BASE_ADDR + ASYSCFG_ASYSCFG_LOCK_OFFSET)

// Register Field Macro Definition
#define ASYSCFG_SLEEP_MODE_SLP_MODE_SHIFT                                        0
#define ASYSCFG_SLEEP_MODE_SLP_MODE_MASK                                         0x00000003
#define ASYSCFG_SLEEP_MODE_SLP_MODE_SET(n)                                       (((uint32_t)(n) << 0  ) & 0x00000003)
#define ASYSCFG_SLEEP_MODE_SLP_MODE_CLR                                          0xFFFFFFFC

#define ASYSCFG_WAIT_CYCLE_PUP_CYCLE_LDO15_DL_HW_EN_SHIFT                        16
#define ASYSCFG_WAIT_CYCLE_PUP_CYCLE_LDO15_DL_HW_EN_MASK                         0x001F0000
#define ASYSCFG_WAIT_CYCLE_PUP_CYCLE_LDO15_DL_HW_EN_SET(n)                       (((uint32_t)(n) << 16 ) & 0x001F0000)
#define ASYSCFG_WAIT_CYCLE_PUP_CYCLE_LDO15_DL_HW_EN_CLR                          0xFFE0FFFF

#define ASYSCFG_WAIT_CYCLE_PUP_CYCLE_LDO33_DL_HW_EN_SHIFT                        8
#define ASYSCFG_WAIT_CYCLE_PUP_CYCLE_LDO33_DL_HW_EN_MASK                         0x00001F00
#define ASYSCFG_WAIT_CYCLE_PUP_CYCLE_LDO33_DL_HW_EN_SET(n)                       (((uint32_t)(n) << 8  ) & 0x00001F00)
#define ASYSCFG_WAIT_CYCLE_PUP_CYCLE_LDO33_DL_HW_EN_CLR                          0xFFFFE0FF

#define ASYSCFG_WAIT_CYCLE_PUP_CYCLE_PSW15_SHIFT                                 4
#define ASYSCFG_WAIT_CYCLE_PUP_CYCLE_PSW15_MASK                                  0x00000070
#define ASYSCFG_WAIT_CYCLE_PUP_CYCLE_PSW15_SET(n)                                (((uint32_t)(n) << 4  ) & 0x00000070)
#define ASYSCFG_WAIT_CYCLE_PUP_CYCLE_PSW15_CLR                                   0xFFFFFF8F

#define ASYSCFG_WAIT_CYCLE_PUP_CYCLE_HFCLK_SHIFT                                 0
#define ASYSCFG_WAIT_CYCLE_PUP_CYCLE_HFCLK_MASK                                  0x0000000F
#define ASYSCFG_WAIT_CYCLE_PUP_CYCLE_HFCLK_SET(n)                                (((uint32_t)(n) << 0  ) & 0x0000000F)
#define ASYSCFG_WAIT_CYCLE_PUP_CYCLE_HFCLK_CLR                                   0xFFFFFFF0

#define ASYSCFG_RCCLK_CTRL_SW_HRC_EN_SHIFT                                       0
#define ASYSCFG_RCCLK_CTRL_SW_HRC_EN_MASK                                        0x00000001
#define ASYSCFG_RCCLK_CTRL_SW_HRC_EN_SET(n)                                      (((uint32_t)(n) << 0  ) & 0x00000001)
#define ASYSCFG_RCCLK_CTRL_SW_HRC_EN_CLR                                         0xFFFFFFFE

#define ASYSCFG_RST_CTRL_M0_LOCKUP_EN_SHIFT                                      16
#define ASYSCFG_RST_CTRL_M0_LOCKUP_EN_MASK                                       0x00010000
#define ASYSCFG_RST_CTRL_M0_LOCKUP_EN_SET(n)                                     (((uint32_t)(n) << 16 ) & 0x00010000)
#define ASYSCFG_RST_CTRL_M0_LOCKUP_EN_CLR                                        0xFFFEFFFF

#define ASYSCFG_RST_CTRL_SW_POR_REQ_SHIFT                                        13
#define ASYSCFG_RST_CTRL_SW_POR_REQ_MASK                                         0x00002000
#define ASYSCFG_RST_CTRL_SW_POR_REQ_SET(n)                                       (((uint32_t)(n) << 13 ) & 0x00002000)
#define ASYSCFG_RST_CTRL_SW_POR_REQ_CLR                                          0xFFFFDFFF

#define ASYSCFG_RST_CTRL_CLR_RST_SHIFT                                           12
#define ASYSCFG_RST_CTRL_CLR_RST_MASK                                            0x00001000
#define ASYSCFG_RST_CTRL_CLR_RST_SET(n)                                          (((uint32_t)(n) << 12 ) & 0x00001000)
#define ASYSCFG_RST_CTRL_CLR_RST_CLR                                             0xFFFFEFFF

#define ASYSCFG_RST_CTRL_RST_FLAG_SHIFT                                          0
#define ASYSCFG_RST_CTRL_RST_FLAG_MASK                                           0x0000003F
#define ASYSCFG_RST_CTRL_RST_FLAG_SET(n)                                         (((uint32_t)(n) << 0  ) & 0x0000003F)
#define ASYSCFG_RST_CTRL_RST_FLAG_CLR                                            0xFFFFFFC0

#define ASYSCFG_VS_LVD_CTRL_VS_LVD_SHIFT                                         8
#define ASYSCFG_VS_LVD_CTRL_VS_LVD_MASK                                          0x00000100
#define ASYSCFG_VS_LVD_CTRL_VS_LVD_SET(n)                                        (((uint32_t)(n) << 8  ) & 0x00000100)
#define ASYSCFG_VS_LVD_CTRL_VS_LVD_CLR                                           0xFFFFFEFF

#define ASYSCFG_VS_LVD_CTRL_VS_LVD_SEL_SHIFT                                     4
#define ASYSCFG_VS_LVD_CTRL_VS_LVD_SEL_MASK                                      0x00000070
#define ASYSCFG_VS_LVD_CTRL_VS_LVD_SEL_SET(n)                                    (((uint32_t)(n) << 4  ) & 0x00000070)
#define ASYSCFG_VS_LVD_CTRL_VS_LVD_SEL_CLR                                       0xFFFFFF8F

#define ASYSCFG_VS_LVD_CTRL_VS_LVD_EN_SHIFT                                      0
#define ASYSCFG_VS_LVD_CTRL_VS_LVD_EN_MASK                                       0x00000001
#define ASYSCFG_VS_LVD_CTRL_VS_LVD_EN_SET(n)                                     (((uint32_t)(n) << 0  ) & 0x00000001)
#define ASYSCFG_VS_LVD_CTRL_VS_LVD_EN_CLR                                        0xFFFFFFFE

#define ASYSCFG_LDO33_CTRL_LDO33_DL_IBASE_SEL_SHIFT                              4
#define ASYSCFG_LDO33_CTRL_LDO33_DL_IBASE_SEL_MASK                               0x00000070
#define ASYSCFG_LDO33_CTRL_LDO33_DL_IBASE_SEL_SET(n)                             (((uint32_t)(n) << 4  ) & 0x00000070)
#define ASYSCFG_LDO33_CTRL_LDO33_DL_IBASE_SEL_CLR                                0xFFFFFF8F

#define ASYSCFG_LDO33_CTRL_LDO33_DL_SW_ENB_SHIFT                                 0
#define ASYSCFG_LDO33_CTRL_LDO33_DL_SW_ENB_MASK                                  0x00000001
#define ASYSCFG_LDO33_CTRL_LDO33_DL_SW_ENB_SET(n)                                (((uint32_t)(n) << 0  ) & 0x00000001)
#define ASYSCFG_LDO33_CTRL_LDO33_DL_SW_ENB_CLR                                   0xFFFFFFFE

#define ASYSCFG_LDO15_CTRL_LDO15_DL_IBASE_SEL_SHIFT                              4
#define ASYSCFG_LDO15_CTRL_LDO15_DL_IBASE_SEL_MASK                               0x00000070
#define ASYSCFG_LDO15_CTRL_LDO15_DL_IBASE_SEL_SET(n)                             (((uint32_t)(n) << 4  ) & 0x00000070)
#define ASYSCFG_LDO15_CTRL_LDO15_DL_IBASE_SEL_CLR                                0xFFFFFF8F

#define ASYSCFG_LDO15_CTRL_LDO15_DL_SW_ENB_SHIFT                                 0
#define ASYSCFG_LDO15_CTRL_LDO15_DL_SW_ENB_MASK                                  0x00000001
#define ASYSCFG_LDO15_CTRL_LDO15_DL_SW_ENB_SET(n)                                (((uint32_t)(n) << 0  ) & 0x00000001)
#define ASYSCFG_LDO15_CTRL_LDO15_DL_SW_ENB_CLR                                   0xFFFFFFFE

#define ASYSCFG_VS_ALT_CTRL_VS_ALT_SEL_SHIFT                                     0
#define ASYSCFG_VS_ALT_CTRL_VS_ALT_SEL_MASK                                      0x00000003
#define ASYSCFG_VS_ALT_CTRL_VS_ALT_SEL_SET(n)                                    (((uint32_t)(n) << 0  ) & 0x00000003)
#define ASYSCFG_VS_ALT_CTRL_VS_ALT_SEL_CLR                                       0xFFFFFFFC

#define ASYSCFG_OPT_STATUS_OTP_OUT_SHIFT                                         0
#define ASYSCFG_OPT_STATUS_OTP_OUT_MASK                                          0x00000001
#define ASYSCFG_OPT_STATUS_OTP_OUT_SET(n)                                        (((uint32_t)(n) << 0  ) & 0x00000001)
#define ASYSCFG_OPT_STATUS_OTP_OUT_CLR                                           0xFFFFFFFE

#define ASYSCFG_PMU_IRQ_CTRL_RESERVED12_OCP_IRQ_MODE_SHIFT                       8
#define ASYSCFG_PMU_IRQ_CTRL_RESERVED12_OCP_IRQ_MODE_MASK                        0x00000700
#define ASYSCFG_PMU_IRQ_CTRL_RESERVED12_OCP_IRQ_MODE_SET(n)                      (((uint32_t)(n) << 8  ) & 0x00000700)
#define ASYSCFG_PMU_IRQ_CTRL_RESERVED12_OCP_IRQ_MODE_CLR                         0xFFFFF8FF

#define ASYSCFG_PMU_IRQ_CTRL_OTP_IRQ_MODE_SHIFT                                  4
#define ASYSCFG_PMU_IRQ_CTRL_OTP_IRQ_MODE_MASK                                   0x00000070
#define ASYSCFG_PMU_IRQ_CTRL_OTP_IRQ_MODE_SET(n)                                 (((uint32_t)(n) << 4  ) & 0x00000070)
#define ASYSCFG_PMU_IRQ_CTRL_OTP_IRQ_MODE_CLR                                    0xFFFFFF8F

#define ASYSCFG_PMU_IRQ_CTRL_VS_LVD_IRQ_MODE_SHIFT                               0
#define ASYSCFG_PMU_IRQ_CTRL_VS_LVD_IRQ_MODE_MASK                                0x00000007
#define ASYSCFG_PMU_IRQ_CTRL_VS_LVD_IRQ_MODE_SET(n)                              (((uint32_t)(n) << 0  ) & 0x00000007)
#define ASYSCFG_PMU_IRQ_CTRL_VS_LVD_IRQ_MODE_CLR                                 0xFFFFFFF8

#define ASYSCFG_IMR_RESERVED12_RDY_INT_MSK_SHIFT                                 4
#define ASYSCFG_IMR_RESERVED12_RDY_INT_MSK_MASK                                  0x00000010
#define ASYSCFG_IMR_RESERVED12_RDY_INT_MSK_SET(n)                                (((uint32_t)(n) << 4  ) & 0x00000010)
#define ASYSCFG_IMR_RESERVED12_RDY_INT_MSK_CLR                                   0xFFFFFFEF

#define ASYSCFG_IMR_RESERVED12_OCP_INT_MSK_SHIFT                                 3
#define ASYSCFG_IMR_RESERVED12_OCP_INT_MSK_MASK                                  0x00000008
#define ASYSCFG_IMR_RESERVED12_OCP_INT_MSK_SET(n)                                (((uint32_t)(n) << 3  ) & 0x00000008)
#define ASYSCFG_IMR_RESERVED12_OCP_INT_MSK_CLR                                   0xFFFFFFF7

#define ASYSCFG_IMR_OTP_INT_MSK_SHIFT                                            2
#define ASYSCFG_IMR_OTP_INT_MSK_MASK                                             0x00000004
#define ASYSCFG_IMR_OTP_INT_MSK_SET(n)                                           (((uint32_t)(n) << 2  ) & 0x00000004)
#define ASYSCFG_IMR_OTP_INT_MSK_CLR                                              0xFFFFFFFB

#define ASYSCFG_IMR_VS_LVD_INT_MSK_SHIFT                                         1
#define ASYSCFG_IMR_VS_LVD_INT_MSK_MASK                                          0x00000002
#define ASYSCFG_IMR_VS_LVD_INT_MSK_SET(n)                                        (((uint32_t)(n) << 1  ) & 0x00000002)
#define ASYSCFG_IMR_VS_LVD_INT_MSK_CLR                                           0xFFFFFFFD

#define ASYSCFG_IMR_WU_INT_MSK_SHIFT                                             0
#define ASYSCFG_IMR_WU_INT_MSK_MASK                                              0x00000001
#define ASYSCFG_IMR_WU_INT_MSK_SET(n)                                            (((uint32_t)(n) << 0  ) & 0x00000001)
#define ASYSCFG_IMR_WU_INT_MSK_CLR                                               0xFFFFFFFE

#define ASYSCFG_ICR_RESERVED12_RDY_INT_CLR_SHIFT                                 4
#define ASYSCFG_ICR_RESERVED12_RDY_INT_CLR_MASK                                  0x00000010
#define ASYSCFG_ICR_RESERVED12_RDY_INT_CLR_SET(n)                                (((uint32_t)(n) << 4  ) & 0x00000010)
#define ASYSCFG_ICR_RESERVED12_RDY_INT_CLR_CLR                                   0xFFFFFFEF

#define ASYSCFG_ICR_RESERVED12_OCP_INT_CLR_SHIFT                                 3
#define ASYSCFG_ICR_RESERVED12_OCP_INT_CLR_MASK                                  0x00000008
#define ASYSCFG_ICR_RESERVED12_OCP_INT_CLR_SET(n)                                (((uint32_t)(n) << 3  ) & 0x00000008)
#define ASYSCFG_ICR_RESERVED12_OCP_INT_CLR_CLR                                   0xFFFFFFF7

#define ASYSCFG_ICR_OTP_INT_CLR_SHIFT                                            2
#define ASYSCFG_ICR_OTP_INT_CLR_MASK                                             0x00000004
#define ASYSCFG_ICR_OTP_INT_CLR_SET(n)                                           (((uint32_t)(n) << 2  ) & 0x00000004)
#define ASYSCFG_ICR_OTP_INT_CLR_CLR                                              0xFFFFFFFB

#define ASYSCFG_ICR_VS_LVD_INT_CLR_SHIFT                                         1
#define ASYSCFG_ICR_VS_LVD_INT_CLR_MASK                                          0x00000002
#define ASYSCFG_ICR_VS_LVD_INT_CLR_SET(n)                                        (((uint32_t)(n) << 1  ) & 0x00000002)
#define ASYSCFG_ICR_VS_LVD_INT_CLR_CLR                                           0xFFFFFFFD

#define ASYSCFG_ICR_WU_INT_CLR_SHIFT                                             0
#define ASYSCFG_ICR_WU_INT_CLR_MASK                                              0x00000001
#define ASYSCFG_ICR_WU_INT_CLR_SET(n)                                            (((uint32_t)(n) << 0  ) & 0x00000001)
#define ASYSCFG_ICR_WU_INT_CLR_CLR                                               0xFFFFFFFE

#define ASYSCFG_ISR_RESERVED12_RDY_INT_STATUS_SHIFT                              4
#define ASYSCFG_ISR_RESERVED12_RDY_INT_STATUS_MASK                               0x00000010
#define ASYSCFG_ISR_RESERVED12_RDY_INT_STATUS_SET(n)                             (((uint32_t)(n) << 4  ) & 0x00000010)
#define ASYSCFG_ISR_RESERVED12_RDY_INT_STATUS_CLR                                0xFFFFFFEF

#define ASYSCFG_ISR_RESERVED12_OCP_INT_STATUS_SHIFT                              3
#define ASYSCFG_ISR_RESERVED12_OCP_INT_STATUS_MASK                               0x00000008
#define ASYSCFG_ISR_RESERVED12_OCP_INT_STATUS_SET(n)                             (((uint32_t)(n) << 3  ) & 0x00000008)
#define ASYSCFG_ISR_RESERVED12_OCP_INT_STATUS_CLR                                0xFFFFFFF7

#define ASYSCFG_ISR_OTP_INT_STATUS_SHIFT                                         2
#define ASYSCFG_ISR_OTP_INT_STATUS_MASK                                          0x00000004
#define ASYSCFG_ISR_OTP_INT_STATUS_SET(n)                                        (((uint32_t)(n) << 2  ) & 0x00000004)
#define ASYSCFG_ISR_OTP_INT_STATUS_CLR                                           0xFFFFFFFB

#define ASYSCFG_ISR_VS_LVD_INT_STATUS_SHIFT                                      1
#define ASYSCFG_ISR_VS_LVD_INT_STATUS_MASK                                       0x00000002
#define ASYSCFG_ISR_VS_LVD_INT_STATUS_SET(n)                                     (((uint32_t)(n) << 1  ) & 0x00000002)
#define ASYSCFG_ISR_VS_LVD_INT_STATUS_CLR                                        0xFFFFFFFD

#define ASYSCFG_ISR_WU_INT_STATUS_SHIFT                                          0
#define ASYSCFG_ISR_WU_INT_STATUS_MASK                                           0x00000001
#define ASYSCFG_ISR_WU_INT_STATUS_SET(n)                                         (((uint32_t)(n) << 0  ) & 0x00000001)
#define ASYSCFG_ISR_WU_INT_STATUS_CLR                                            0xFFFFFFFE

#define ASYSCFG_IRSR_RESERVED12_RDY_INT_RAW_STATUS_SHIFT                         4
#define ASYSCFG_IRSR_RESERVED12_RDY_INT_RAW_STATUS_MASK                          0x00000010
#define ASYSCFG_IRSR_RESERVED12_RDY_INT_RAW_STATUS_SET(n)                        (((uint32_t)(n) << 4  ) & 0x00000010)
#define ASYSCFG_IRSR_RESERVED12_RDY_INT_RAW_STATUS_CLR                           0xFFFFFFEF

#define ASYSCFG_IRSR_RESERVED12_OCP_INT_RAW_STATUS_SHIFT                         3
#define ASYSCFG_IRSR_RESERVED12_OCP_INT_RAW_STATUS_MASK                          0x00000008
#define ASYSCFG_IRSR_RESERVED12_OCP_INT_RAW_STATUS_SET(n)                        (((uint32_t)(n) << 3  ) & 0x00000008)
#define ASYSCFG_IRSR_RESERVED12_OCP_INT_RAW_STATUS_CLR                           0xFFFFFFF7

#define ASYSCFG_IRSR_OTP_INT_RAW_STATUS_SHIFT                                    2
#define ASYSCFG_IRSR_OTP_INT_RAW_STATUS_MASK                                     0x00000004
#define ASYSCFG_IRSR_OTP_INT_RAW_STATUS_SET(n)                                   (((uint32_t)(n) << 2  ) & 0x00000004)
#define ASYSCFG_IRSR_OTP_INT_RAW_STATUS_CLR                                      0xFFFFFFFB

#define ASYSCFG_IRSR_VS_LVD_INT_RAW_STATUS_SHIFT                                 1
#define ASYSCFG_IRSR_VS_LVD_INT_RAW_STATUS_MASK                                  0x00000002
#define ASYSCFG_IRSR_VS_LVD_INT_RAW_STATUS_SET(n)                                (((uint32_t)(n) << 1  ) & 0x00000002)
#define ASYSCFG_IRSR_VS_LVD_INT_RAW_STATUS_CLR                                   0xFFFFFFFD

#define ASYSCFG_IRSR_WU_INT_RAW_STATUS_SHIFT                                     0
#define ASYSCFG_IRSR_WU_INT_RAW_STATUS_MASK                                      0x00000001
#define ASYSCFG_IRSR_WU_INT_RAW_STATUS_SET(n)                                    (((uint32_t)(n) << 0  ) & 0x00000001)
#define ASYSCFG_IRSR_WU_INT_RAW_STATUS_CLR                                       0xFFFFFFFE

#define ASYSCFG_WU_CTRL_WU_FILTER_CNT_MAX_SHIFT                                  8
#define ASYSCFG_WU_CTRL_WU_FILTER_CNT_MAX_MASK                                   0x00000300
#define ASYSCFG_WU_CTRL_WU_FILTER_CNT_MAX_SET(n)                                 (((uint32_t)(n) << 8  ) & 0x00000300)
#define ASYSCFG_WU_CTRL_WU_FILTER_CNT_MAX_CLR                                    0xFFFFFCFF

#define ASYSCFG_WU_CTRL_WU_TIM_SHIFT                                             4
#define ASYSCFG_WU_CTRL_WU_TIM_MASK                                              0x000000F0
#define ASYSCFG_WU_CTRL_WU_TIM_SET(n)                                            (((uint32_t)(n) << 4  ) & 0x000000F0)
#define ASYSCFG_WU_CTRL_WU_TIM_CLR                                               0xFFFFFF0F

#define ASYSCFG_WU_CTRL_WU_SRC_SEL_SHIFT                                         1
#define ASYSCFG_WU_CTRL_WU_SRC_SEL_MASK                                          0x00000002
#define ASYSCFG_WU_CTRL_WU_SRC_SEL_SET(n)                                        (((uint32_t)(n) << 1  ) & 0x00000002)
#define ASYSCFG_WU_CTRL_WU_SRC_SEL_CLR                                           0xFFFFFFFD

#define ASYSCFG_WU_CTRL_WU_EN_SHIFT                                              0
#define ASYSCFG_WU_CTRL_WU_EN_MASK                                               0x00000001
#define ASYSCFG_WU_CTRL_WU_EN_SET(n)                                             (((uint32_t)(n) << 0  ) & 0x00000001)
#define ASYSCFG_WU_CTRL_WU_EN_CLR                                                0xFFFFFFFE

#define ASYSCFG_ADC_CAL_CTRL_ADC_OUT_OFFSET_SHIFT                                16
#define ASYSCFG_ADC_CAL_CTRL_ADC_OUT_OFFSET_MASK                                 0x3FFF0000
#define ASYSCFG_ADC_CAL_CTRL_ADC_OUT_OFFSET_SET(n)                               (((uint32_t)(n) << 16 ) & 0x3FFF0000)
#define ASYSCFG_ADC_CAL_CTRL_ADC_OUT_OFFSET_CLR                                  0xC000FFFF

#define ASYSCFG_ADC_CAL_CTRL_ADC_OUT_CAL_SHIFT                                   0
#define ASYSCFG_ADC_CAL_CTRL_ADC_OUT_CAL_MASK                                    0x00003FFF
#define ASYSCFG_ADC_CAL_CTRL_ADC_OUT_CAL_SET(n)                                  (((uint32_t)(n) << 0  ) & 0x00003FFF)
#define ASYSCFG_ADC_CAL_CTRL_ADC_OUT_CAL_CLR                                     0xFFFFC000

#define ASYSCFG_OTP_RST_CTRL_OTP_RST_EN_SHIFT                                    0
#define ASYSCFG_OTP_RST_CTRL_OTP_RST_EN_MASK                                     0x00000001
#define ASYSCFG_OTP_RST_CTRL_OTP_RST_EN_SET(n)                                   (((uint32_t)(n) << 0  ) & 0x00000001)
#define ASYSCFG_OTP_RST_CTRL_OTP_RST_EN_CLR                                      0xFFFFFFFE

#define ASYSCFG_IO4_RST_CTRL_IO4_RST_EN_SHIFT                                    0
#define ASYSCFG_IO4_RST_CTRL_IO4_RST_EN_MASK                                     0x00000001
#define ASYSCFG_IO4_RST_CTRL_IO4_RST_EN_SET(n)                                   (((uint32_t)(n) << 0  ) & 0x00000001)
#define ASYSCFG_IO4_RST_CTRL_IO4_RST_EN_CLR                                      0xFFFFFFFE

#define ASYSCFG_RESERVED12_CTRL_RESERVED12_HW_EN_CLR_SHIFT                       21
#define ASYSCFG_RESERVED12_CTRL_RESERVED12_HW_EN_CLR_MASK                        0x00200000
#define ASYSCFG_RESERVED12_CTRL_RESERVED12_HW_EN_CLR_SET(n)                      (((uint32_t)(n) << 21 ) & 0x00200000)
#define ASYSCFG_RESERVED12_CTRL_RESERVED12_HW_EN_CLR_CLR                         0xFFDFFFFF

#define ASYSCFG_RESERVED12_CTRL_RESERVED12_WKUP_EN_SHIFT                         20
#define ASYSCFG_RESERVED12_CTRL_RESERVED12_WKUP_EN_MASK                          0x00100000
#define ASYSCFG_RESERVED12_CTRL_RESERVED12_WKUP_EN_SET(n)                        (((uint32_t)(n) << 20 ) & 0x00100000)
#define ASYSCFG_RESERVED12_CTRL_RESERVED12_WKUP_EN_CLR                           0xFFEFFFFF

#define ASYSCFG_RESERVED12_CTRL_RESERVED12_RDY_CNT_MAX_SHIFT                     12
#define ASYSCFG_RESERVED12_CTRL_RESERVED12_RDY_CNT_MAX_MASK                      0x0003F000
#define ASYSCFG_RESERVED12_CTRL_RESERVED12_RDY_CNT_MAX_SET(n)                    (((uint32_t)(n) << 12 ) & 0x0003F000)
#define ASYSCFG_RESERVED12_CTRL_RESERVED12_RDY_CNT_MAX_CLR                       0xFFFC0FFF

#define ASYSCFG_RESERVED12_CTRL_RESERVED12_OCP_SHIFT                             8
#define ASYSCFG_RESERVED12_CTRL_RESERVED12_OCP_MASK                              0x00000100
#define ASYSCFG_RESERVED12_CTRL_RESERVED12_OCP_SET(n)                            (((uint32_t)(n) << 8  ) & 0x00000100)
#define ASYSCFG_RESERVED12_CTRL_RESERVED12_OCP_CLR                               0xFFFFFEFF

#define ASYSCFG_RESERVED12_CTRL_RESERVED12_OCP_DISABLE_SHIFT                     4
#define ASYSCFG_RESERVED12_CTRL_RESERVED12_OCP_DISABLE_MASK                      0x00000010
#define ASYSCFG_RESERVED12_CTRL_RESERVED12_OCP_DISABLE_SET(n)                    (((uint32_t)(n) << 4  ) & 0x00000010)
#define ASYSCFG_RESERVED12_CTRL_RESERVED12_OCP_DISABLE_CLR                       0xFFFFFFEF

#define ASYSCFG_RESERVED12_CTRL_RESERVED12_PULL_DOWN_SHIFT                       1
#define ASYSCFG_RESERVED12_CTRL_RESERVED12_PULL_DOWN_MASK                        0x00000002
#define ASYSCFG_RESERVED12_CTRL_RESERVED12_PULL_DOWN_SET(n)                      (((uint32_t)(n) << 1  ) & 0x00000002)
#define ASYSCFG_RESERVED12_CTRL_RESERVED12_PULL_DOWN_CLR                         0xFFFFFFFD

#define ASYSCFG_RESERVED12_CTRL_RESERVED12_EN_SHIFT                              0
#define ASYSCFG_RESERVED12_CTRL_RESERVED12_EN_MASK                               0x00000001
#define ASYSCFG_RESERVED12_CTRL_RESERVED12_EN_SET(n)                             (((uint32_t)(n) << 0  ) & 0x00000001)
#define ASYSCFG_RESERVED12_CTRL_RESERVED12_EN_CLR                                0xFFFFFFFE

#define ASYSCFG_IO4_RST_FILTER_IO4_RST_FILTER_CNT_MAX_SHIFT                      0
#define ASYSCFG_IO4_RST_FILTER_IO4_RST_FILTER_CNT_MAX_MASK                       0x00001FFF
#define ASYSCFG_IO4_RST_FILTER_IO4_RST_FILTER_CNT_MAX_SET(n)                     (((uint32_t)(n) << 0  ) & 0x00001FFF)
#define ASYSCFG_IO4_RST_FILTER_IO4_RST_FILTER_CNT_MAX_CLR                        0xFFFFE000

#define ASYSCFG_AON_IO_CTRL_AON_IO_OUT_VAL_SHIFT                                 4
#define ASYSCFG_AON_IO_CTRL_AON_IO_OUT_VAL_MASK                                  0x00000010
#define ASYSCFG_AON_IO_CTRL_AON_IO_OUT_VAL_SET(n)                                (((uint32_t)(n) << 4  ) & 0x00000010)
#define ASYSCFG_AON_IO_CTRL_AON_IO_OUT_VAL_CLR                                   0xFFFFFFEF

#define ASYSCFG_AON_IO_CTRL_AON_IO_OUT_SEL_SHIFT                                 1
#define ASYSCFG_AON_IO_CTRL_AON_IO_OUT_SEL_MASK                                  0x00000006
#define ASYSCFG_AON_IO_CTRL_AON_IO_OUT_SEL_SET(n)                                (((uint32_t)(n) << 1  ) & 0x00000006)
#define ASYSCFG_AON_IO_CTRL_AON_IO_OUT_SEL_CLR                                   0xFFFFFFF9

#define ASYSCFG_AON_IO_CTRL_AON_IO_OUT_EN_SHIFT                                  0
#define ASYSCFG_AON_IO_CTRL_AON_IO_OUT_EN_MASK                                   0x00000001
#define ASYSCFG_AON_IO_CTRL_AON_IO_OUT_EN_SET(n)                                 (((uint32_t)(n) << 0  ) & 0x00000001)
#define ASYSCFG_AON_IO_CTRL_AON_IO_OUT_EN_CLR                                    0xFFFFFFFE

#define ASYSCFG_BKUP0_BKUP0_SHIFT                                                0
#define ASYSCFG_BKUP0_BKUP0_MASK                                                 0xFFFFFFFF
#define ASYSCFG_BKUP0_BKUP0_SET(n)                                               (((uint32_t)(n) << 0  ) & 0xFFFFFFFF)
#define ASYSCFG_BKUP0_BKUP0_CLR                                                  0x00000000

#define ASYSCFG_BKUP1_BKUP1_SHIFT                                                0
#define ASYSCFG_BKUP1_BKUP1_MASK                                                 0xFFFFFFFF
#define ASYSCFG_BKUP1_BKUP1_SET(n)                                               (((uint32_t)(n) << 0  ) & 0xFFFFFFFF)
#define ASYSCFG_BKUP1_BKUP1_CLR                                                  0x00000000

#define ASYSCFG_PATCH_BIT_PATCH_BIT_SHIFT                                        0
#define ASYSCFG_PATCH_BIT_PATCH_BIT_MASK                                         0x0000000F
#define ASYSCFG_PATCH_BIT_PATCH_BIT_SET(n)                                       (((uint32_t)(n) << 0  ) & 0x0000000F)
#define ASYSCFG_PATCH_BIT_PATCH_BIT_CLR                                          0xFFFFFFF0

#define ASYSCFG_ASYSCFG_LOCK_ASYSCFG_LOCK_SHIFT                                  0
#define ASYSCFG_ASYSCFG_LOCK_ASYSCFG_LOCK_MASK                                   0x00000001
#define ASYSCFG_ASYSCFG_LOCK_ASYSCFG_LOCK_SET(n)                                 (((uint32_t)(n) << 0  ) & 0x00000001)
#define ASYSCFG_ASYSCFG_LOCK_ASYSCFG_LOCK_CLR                                    0xFFFFFFFE

// Register Structure Definition
typedef struct
{
    __IO uint32_t SLP_MODE                  : 2  ; // 1  : 0
    __IO uint32_t RESERVED0                 : 30 ; // 31 : 2
} ASYSCFG_SLEEP_MODE_FIELD_T;

typedef struct
{
    __IO uint32_t PUP_CYCLE_HFCLK           : 4  ; // 3  : 0
    __IO uint32_t PUP_CYCLE_PSW15           : 3  ; // 6  : 4
    __IO uint32_t RESERVED2                 : 1  ; // 7  : 7
    __IO uint32_t PUP_CYCLE_LDO33_DL_HW_EN  : 5  ; // 12 : 8
    __IO uint32_t RESERVED1                 : 3  ; // 15 : 13
    __IO uint32_t PUP_CYCLE_LDO15_DL_HW_EN  : 5  ; // 20 : 16
    __IO uint32_t RESERVED0                 : 11 ; // 31 : 21
} ASYSCFG_WAIT_CYCLE_FIELD_T;

typedef struct
{
    __IO uint32_t SW_HRC_EN                 : 1  ; // 0  : 0
    __IO uint32_t RESERVED0                 : 31 ; // 31 : 1
} ASYSCFG_RCCLK_CTRL_FIELD_T;

typedef struct
{
    __IO uint32_t RST_FLAG                  : 6  ; // 5  : 0
    __IO uint32_t RESERVED2                 : 6  ; // 11 : 6
    __IO uint32_t CLR_RST                   : 1  ; // 12 : 12
    __IO uint32_t SW_POR_REQ                : 1  ; // 13 : 13
    __IO uint32_t RESERVED1                 : 2  ; // 15 : 14
    __IO uint32_t M0_LOCKUP_EN              : 1  ; // 16 : 16
    __IO uint32_t RESERVED0                 : 15 ; // 31 : 17
} ASYSCFG_RST_CTRL_FIELD_T;

typedef struct
{
    __IO uint32_t VS_LVD_EN                 : 1  ; // 0  : 0
    __IO uint32_t RESERVED2                 : 3  ; // 3  : 1
    __IO uint32_t VS_LVD_SEL                : 3  ; // 6  : 4
    __IO uint32_t RESERVED1                 : 1  ; // 7  : 7
    __IO uint32_t VS_LVD                    : 1  ; // 8  : 8
    __IO uint32_t RESERVED0                 : 23 ; // 31 : 9
} ASYSCFG_VS_LVD_CTRL_FIELD_T;

typedef struct
{
    __IO uint32_t LDO33_DL_SW_ENB           : 1  ; // 0  : 0
    __IO uint32_t RESERVED1                 : 3  ; // 3  : 1
    __IO uint32_t LDO33_DL_IBASE_SEL        : 3  ; // 6  : 4
    __IO uint32_t RESERVED0                 : 25 ; // 31 : 7
} ASYSCFG_LDO33_CTRL_FIELD_T;

typedef struct
{
    __IO uint32_t LDO15_DL_SW_ENB           : 1  ; // 0  : 0
    __IO uint32_t RESERVED1                 : 3  ; // 3  : 1
    __IO uint32_t LDO15_DL_IBASE_SEL        : 3  ; // 6  : 4
    __IO uint32_t RESERVED0                 : 25 ; // 31 : 7
} ASYSCFG_LDO15_CTRL_FIELD_T;

typedef struct
{
    __IO uint32_t VS_ALT_SEL                : 2  ; // 1  : 0
    __IO uint32_t RESERVED0                 : 30 ; // 31 : 2
} ASYSCFG_VS_ALT_CTRL_FIELD_T;

typedef struct
{
    __IO uint32_t OTP_OUT                   : 1  ; // 0  : 0
    __IO uint32_t RESERVED0                 : 31 ; // 31 : 1
} ASYSCFG_OPT_STATUS_FIELD_T;

typedef struct
{
    __IO uint32_t VS_LVD_IRQ_MODE           : 3  ; // 2  : 0
    __IO uint32_t RESERVED2                 : 1  ; // 3  : 3
    __IO uint32_t OTP_IRQ_MODE              : 3  ; // 6  : 4
    __IO uint32_t RESERVED1                 : 1  ; // 7  : 7
    __IO uint32_t RESERVED12_OCP_IRQ_MODE   : 3  ; // 10 : 8
    __IO uint32_t RESERVED0                 : 21 ; // 31 : 11
} ASYSCFG_PMU_IRQ_CTRL_FIELD_T;

typedef struct
{
    __IO uint32_t WU_INT_MSK                : 1  ; // 0  : 0
    __IO uint32_t VS_LVD_INT_MSK            : 1  ; // 1  : 1
    __IO uint32_t OTP_INT_MSK               : 1  ; // 2  : 2
    __IO uint32_t RESERVED12_OCP_INT_MSK    : 1  ; // 3  : 3
    __IO uint32_t RESERVED12_RDY_INT_MSK    : 1  ; // 4  : 4
    __IO uint32_t RESERVED0                 : 27 ; // 31 : 5
} ASYSCFG_IMR_FIELD_T;

typedef struct
{
    __IO uint32_t WU_INT_CLR                : 1  ; // 0  : 0
    __IO uint32_t VS_LVD_INT_CLR            : 1  ; // 1  : 1
    __IO uint32_t OTP_INT_CLR               : 1  ; // 2  : 2
    __IO uint32_t RESERVED12_OCP_INT_CLR    : 1  ; // 3  : 3
    __IO uint32_t RESERVED12_RDY_INT_CLR    : 1  ; // 4  : 4
    __IO uint32_t RESERVED0                 : 27 ; // 31 : 5
} ASYSCFG_ICR_FIELD_T;

typedef struct
{
    __IO uint32_t WU_INT_STATUS             : 1  ; // 0  : 0
    __IO uint32_t VS_LVD_INT_STATUS         : 1  ; // 1  : 1
    __IO uint32_t OTP_INT_STATUS            : 1  ; // 2  : 2
    __IO uint32_t RESERVED12_OCP_INT_STATUS : 1  ; // 3  : 3
    __IO uint32_t RESERVED12_RDY_INT_STATUS : 1  ; // 4  : 4
    __IO uint32_t RESERVED0                 : 27 ; // 31 : 5
} ASYSCFG_ISR_FIELD_T;

typedef struct
{
    __IO uint32_t WU_INT_RAW_STATUS         : 1  ; // 0  : 0
    __IO uint32_t VS_LVD_INT_RAW_STATUS     : 1  ; // 1  : 1
    __IO uint32_t OTP_INT_RAW_STATUS        : 1  ; // 2  : 2
    __IO uint32_t RESERVED12_OCP_INT_RAW_STATUS : 1  ; // 3  : 3
    __IO uint32_t RESERVED12_RDY_INT_RAW_STATUS : 1  ; // 4  : 4
    __IO uint32_t RESERVED0                 : 27 ; // 31 : 5
} ASYSCFG_IRSR_FIELD_T;

typedef struct
{
    __IO uint32_t WU_EN                     : 1  ; // 0  : 0
    __IO uint32_t WU_SRC_SEL                : 1  ; // 1  : 1
    __IO uint32_t RESERVED1                 : 2  ; // 3  : 2
    __IO uint32_t WU_TIM                    : 4  ; // 7  : 4
    __IO uint32_t WU_FILTER_CNT_MAX         : 2  ; // 9  : 8
    __IO uint32_t RESERVED0                 : 22 ; // 31 : 10
} ASYSCFG_WU_CTRL_FIELD_T;

typedef struct
{
    __IO uint32_t ADC_OUT_CAL               : 14 ; // 13 : 0
    __IO uint32_t RESERVED1                 : 2  ; // 15 : 14
    __IO uint32_t ADC_OUT_OFFSET            : 14 ; // 29 : 16
    __IO uint32_t RESERVED0                 : 2  ; // 31 : 30
} ASYSCFG_ADC_CAL_CTRL_FIELD_T;

typedef struct
{
    __IO uint32_t OTP_RST_EN                : 1  ; // 0  : 0
    __IO uint32_t RESERVED0                 : 31 ; // 31 : 1
} ASYSCFG_OTP_RST_CTRL_FIELD_T;

typedef struct
{
    __IO uint32_t IO4_RST_EN                : 1  ; // 0  : 0
    __IO uint32_t RESERVED0                 : 31 ; // 31 : 1
} ASYSCFG_IO4_RST_CTRL_FIELD_T;

typedef struct
{
    __IO uint32_t RESERVED12_EN             : 1  ; // 0  : 0
    __IO uint32_t RESERVED12_PULL_DOWN      : 1  ; // 1  : 1
    __IO uint32_t RESERVED4                 : 2  ; // 3  : 2
    __IO uint32_t RESERVED12_OCP_DISABLE    : 1  ; // 4  : 4
    __IO uint32_t RESERVED3                 : 3  ; // 7  : 5
    __IO uint32_t RESERVED12_OCP            : 1  ; // 8  : 8
    __IO uint32_t RESERVED2                 : 3  ; // 11 : 9
    __IO uint32_t RESERVED12_RDY_CNT_MAX    : 6  ; // 17 : 12
    __IO uint32_t RESERVED1                 : 2  ; // 19 : 18
    __IO uint32_t RESERVED12_WKUP_EN        : 1  ; // 20 : 20
    __IO uint32_t RESERVED12_HW_EN_CLR      : 1  ; // 21 : 21
    __IO uint32_t RESERVED0                 : 10 ; // 31 : 22
} ASYSCFG_RESERVED12_CTRL_FIELD_T;

typedef struct
{
    __IO uint32_t IO4_RST_FILTER_CNT_MAX    : 13 ; // 12 : 0
    __IO uint32_t RESERVED0                 : 19 ; // 31 : 13
} ASYSCFG_IO4_RST_FILTER_FIELD_T;

typedef struct
{
    __IO uint32_t AON_IO_OUT_EN             : 1  ; // 0  : 0
    __IO uint32_t AON_IO_OUT_SEL            : 2  ; // 2  : 1
    __IO uint32_t RESERVED1                 : 1  ; // 3  : 3
    __IO uint32_t AON_IO_OUT_VAL            : 1  ; // 4  : 4
    __IO uint32_t RESERVED0                 : 27 ; // 31 : 5
} ASYSCFG_AON_IO_CTRL_FIELD_T;

typedef struct
{
    __IO uint32_t BKUP0                     : 32 ; // 31 : 0
} ASYSCFG_BKUP0_FIELD_T;

typedef struct
{
    __IO uint32_t BKUP1                     : 32 ; // 31 : 0
} ASYSCFG_BKUP1_FIELD_T;

typedef struct
{
    __IO uint32_t PATCH_BIT                 : 4  ; // 3  : 0
    __IO uint32_t RESERVED0                 : 28 ; // 31 : 4
} ASYSCFG_PATCH_BIT_FIELD_T;

typedef struct
{
    __IO uint32_t ASYSCFG_LOCK              : 1  ; // 0  : 0
    __IO uint32_t RESERVED0                 : 31 ; // 31 : 1
} ASYSCFG_ASYSCFG_LOCK_FIELD_T;

// Register Map Structure Definition
typedef struct
{
    union
    {
        __IO  uint32_t SLEEP_MODE                       ; // 0x0000
        __IO  ASYSCFG_SLEEP_MODE_FIELD_T SLEEP_MODE_F   ;
    };

    union
    {
        __IO  uint32_t WAIT_CYCLE                       ; // 0x0004
        __IO  ASYSCFG_WAIT_CYCLE_FIELD_T WAIT_CYCLE_F   ;
    };

    union
    {
        __IO  uint32_t RCCLK_CTRL                       ; // 0x0008
        __IO  ASYSCFG_RCCLK_CTRL_FIELD_T RCCLK_CTRL_F   ;
    };

    union
    {
        __IO  uint32_t RST_CTRL                         ; // 0x000C
        __IO  ASYSCFG_RST_CTRL_FIELD_T RST_CTRL_F       ;
    };

    union
    {
        __IO  uint32_t VS_LVD_CTRL                      ; // 0x0010
        __IO  ASYSCFG_VS_LVD_CTRL_FIELD_T VS_LVD_CTRL_F ;
    };

    union
    {
        __IO  uint32_t LDO33_CTRL                       ; // 0x0014
        __IO  ASYSCFG_LDO33_CTRL_FIELD_T LDO33_CTRL_F   ;
    };

    union
    {
        __IO  uint32_t LDO15_CTRL                       ; // 0x0018
        __IO  ASYSCFG_LDO15_CTRL_FIELD_T LDO15_CTRL_F   ;
    };

    union
    {
        __IO  uint32_t VS_ALT_CTRL                      ; // 0x001C
        __IO  ASYSCFG_VS_ALT_CTRL_FIELD_T VS_ALT_CTRL_F ;
    };

    union
    {
        __IO  uint32_t OPT_STATUS                       ; // 0x0020
        __IO  ASYSCFG_OPT_STATUS_FIELD_T OPT_STATUS_F   ;
    };

    union
    {
        __IO  uint32_t PMU_IRQ_CTRL                     ; // 0x0024
        __IO  ASYSCFG_PMU_IRQ_CTRL_FIELD_T PMU_IRQ_CTRL_F;
    };

    union
    {
        __IO  uint32_t IMR                              ; // 0x0028
        __IO  ASYSCFG_IMR_FIELD_T IMR_F                 ;
    };

    union
    {
        __IO  uint32_t ICR                              ; // 0x002C
        __IO  ASYSCFG_ICR_FIELD_T ICR_F                 ;
    };

    union
    {
        __IO  uint32_t ISR                              ; // 0x0030
        __IO  ASYSCFG_ISR_FIELD_T ISR_F                 ;
    };

    union
    {
        __IO  uint32_t IRSR                             ; // 0x0034
        __IO  ASYSCFG_IRSR_FIELD_T IRSR_F               ;
    };

    union
    {
        __IO  uint32_t WU_CTRL                          ; // 0x0038
        __IO  ASYSCFG_WU_CTRL_FIELD_T WU_CTRL_F         ;
    };

    union
    {
        __IO  uint32_t ADC_CAL_CTRL                     ; // 0x003C
        __IO  ASYSCFG_ADC_CAL_CTRL_FIELD_T ADC_CAL_CTRL_F;
    };

    union
    {
        __IO  uint32_t OTP_RST_CTRL                     ; // 0x0040
        __IO  ASYSCFG_OTP_RST_CTRL_FIELD_T OTP_RST_CTRL_F;
    };

    union
    {
        __IO  uint32_t IO4_RST_CTRL                     ; // 0x0044
        __IO  ASYSCFG_IO4_RST_CTRL_FIELD_T IO4_RST_CTRL_F;
    };

    union
    {
        __IO  uint32_t RESERVED12_CTRL                  ; // 0x0048
        __IO  ASYSCFG_RESERVED12_CTRL_FIELD_T RESERVED12_CTRL_F;
    };

    union
    {
        __IO  uint32_t IO4_RST_FILTER                   ; // 0x004C
        __IO  ASYSCFG_IO4_RST_FILTER_FIELD_T IO4_RST_FILTER_F;
    };

    union
    {
        __IO  uint32_t AON_IO_CTRL                      ; // 0x0050
        __IO  ASYSCFG_AON_IO_CTRL_FIELD_T AON_IO_CTRL_F ;
    };

    uint32_t RESERVED0[39]                              ;

    union
    {
        __IO  uint32_t BKUP0                            ; // 0x00F0
        __IO  ASYSCFG_BKUP0_FIELD_T BKUP0_F             ;
    };

    union
    {
        __IO  uint32_t BKUP1                            ; // 0x00F4
        __IO  ASYSCFG_BKUP1_FIELD_T BKUP1_F             ;
    };

    union
    {
        __IO  uint32_t PATCH_BIT                        ; // 0x00F8
        __IO  ASYSCFG_PATCH_BIT_FIELD_T PATCH_BIT_F     ;
    };

    union
    {
        __IO  uint32_t ASYSCFG_LOCK                     ; // 0x00FC
        __IO  ASYSCFG_ASYSCFG_LOCK_FIELD_T ASYSCFG_LOCK_F;
    };

} ASYSCFG_REG_TypeDef;

#endif /* __ASYSCFG_DEF_H__ */
