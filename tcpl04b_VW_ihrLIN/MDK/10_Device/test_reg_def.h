/**
 *****************************************************************************
 * @brief   test_reg_def header file.
 *
 * @file    test_reg_def.h
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

#ifndef __TEST_DEF_H__
#define __TEST_DEF_H__

// Register Offset Macro Definition
#define TEST_TEST_MODE_CTRL_OFFSET                              0x0000
#define TEST_TEST_SWD_CTRL_OFFSET                               0x0004
#define TEST_TEST_PMU_CTRL_OFFSET                               0x0008
#define TEST_TEST_HRC48M_TRIM_OFFSET                            0x000C
#define TEST_TEST_LRC32K_TRIM_OFFSET                            0x0010
#define TEST_TEST_LED_TRIM_OFFSET                               0x0014
#define TEST_TEST_ANA_CTRL_OFFSET                               0x0018
#define TEST_TEST_TPREF_TRIM_OFFSET                             0x001C
#define TEST_TEST_LDO33_TRIM_OFFSET                             0x0020
#define TEST_TEST_OTP_TRIM_OFFSET                               0x0024
#define TEST_TEST_LIN_TRIM_OFFSET                               0x0028
#define TEST_TEST_LIN_CTRL_OFFSET                               0x002C
#define TEST_TEST_LIN_STATUS_OFFSET                             0x0030
#define TEST_TEST_ADC_CTRL_OFFSET                               0x0034
#define TEST_TEST_BUS_CTRL_OFFSET                               0x0038
#define TEST_TEST_LDO15_TRIM_OFFSET                             0x003C
#define TEST_TM_STATUS_OFFSET                                   0x0040
#define TEST_TEST_LED_IB_DIAG_TRIM_OFFSET                       0x0044
#define TEST_TEST_BIAS_TRIM_OFFSET                              0x0048
#define TEST_TEST_LED_LDO5_TRIM_OFFSET                          0x004C
#define TEST_TEST_RESERVED_T_TRIM_OFFSET                        0x0050
#define TEST_ADC_TRIM0_OFFSET                                   0x0080
#define TEST_ADC_TRIM1_OFFSET                                   0x0084
#define TEST_ADC_TRIM2_OFFSET                                   0x0088
#define TEST_ADC_TRIM3_OFFSET                                   0x008C
#define TEST_ADC_TRIM4_OFFSET                                   0x0090
#define TEST_ADC_TRIM5_OFFSET                                   0x0094
#define TEST_ADC_TRIM6_OFFSET                                   0x0098
#define TEST_ADC_TRIM7_OFFSET                                   0x009C
#define TEST_TEST_LOCK_OFFSET                                   0x00FC

// Register Address Macro Definition
#define TEST_TEST_MODE_CTRL_ADDR                                ( TEST_BASE_ADDR + TEST_TEST_MODE_CTRL_OFFSET)
#define TEST_TEST_SWD_CTRL_ADDR                                 ( TEST_BASE_ADDR + TEST_TEST_SWD_CTRL_OFFSET)
#define TEST_TEST_PMU_CTRL_ADDR                                 ( TEST_BASE_ADDR + TEST_TEST_PMU_CTRL_OFFSET)
#define TEST_TEST_HRC48M_TRIM_ADDR                              ( TEST_BASE_ADDR + TEST_TEST_HRC48M_TRIM_OFFSET)
#define TEST_TEST_LRC32K_TRIM_ADDR                              ( TEST_BASE_ADDR + TEST_TEST_LRC32K_TRIM_OFFSET)
#define TEST_TEST_LED_TRIM_ADDR                                 ( TEST_BASE_ADDR + TEST_TEST_LED_TRIM_OFFSET)
#define TEST_TEST_ANA_CTRL_ADDR                                 ( TEST_BASE_ADDR + TEST_TEST_ANA_CTRL_OFFSET)
#define TEST_TEST_TPREF_TRIM_ADDR                               ( TEST_BASE_ADDR + TEST_TEST_TPREF_TRIM_OFFSET)
#define TEST_TEST_LDO33_TRIM_ADDR                               ( TEST_BASE_ADDR + TEST_TEST_LDO33_TRIM_OFFSET)
#define TEST_TEST_OTP_TRIM_ADDR                                 ( TEST_BASE_ADDR + TEST_TEST_OTP_TRIM_OFFSET)
#define TEST_TEST_LIN_TRIM_ADDR                                 ( TEST_BASE_ADDR + TEST_TEST_LIN_TRIM_OFFSET)
#define TEST_TEST_LIN_CTRL_ADDR                                 ( TEST_BASE_ADDR + TEST_TEST_LIN_CTRL_OFFSET)
#define TEST_TEST_LIN_STATUS_ADDR                               ( TEST_BASE_ADDR + TEST_TEST_LIN_STATUS_OFFSET)
#define TEST_TEST_ADC_CTRL_ADDR                                 ( TEST_BASE_ADDR + TEST_TEST_ADC_CTRL_OFFSET)
#define TEST_TEST_BUS_CTRL_ADDR                                 ( TEST_BASE_ADDR + TEST_TEST_BUS_CTRL_OFFSET)
#define TEST_TEST_LDO15_TRIM_ADDR                               ( TEST_BASE_ADDR + TEST_TEST_LDO15_TRIM_OFFSET)
#define TEST_TM_STATUS_ADDR                                     ( TEST_BASE_ADDR + TEST_TM_STATUS_OFFSET)
#define TEST_TEST_LED_IB_DIAG_TRIM_ADDR                         ( TEST_BASE_ADDR + TEST_TEST_LED_IB_DIAG_TRIM_OFFSET)
#define TEST_TEST_BIAS_TRIM_ADDR                                ( TEST_BASE_ADDR + TEST_TEST_BIAS_TRIM_OFFSET)
#define TEST_TEST_LED_LDO5_TRIM_ADDR                            ( TEST_BASE_ADDR + TEST_TEST_LED_LDO5_TRIM_OFFSET)
#define TEST_TEST_RESERVED_T_TRIM_ADDR                          ( TEST_BASE_ADDR + TEST_TEST_RESERVED_T_TRIM_OFFSET)
#define TEST_ADC_TRIM0_ADDR                                     ( TEST_BASE_ADDR + TEST_ADC_TRIM0_OFFSET)
#define TEST_ADC_TRIM1_ADDR                                     ( TEST_BASE_ADDR + TEST_ADC_TRIM1_OFFSET)
#define TEST_ADC_TRIM2_ADDR                                     ( TEST_BASE_ADDR + TEST_ADC_TRIM2_OFFSET)
#define TEST_ADC_TRIM3_ADDR                                     ( TEST_BASE_ADDR + TEST_ADC_TRIM3_OFFSET)
#define TEST_ADC_TRIM4_ADDR                                     ( TEST_BASE_ADDR + TEST_ADC_TRIM4_OFFSET)
#define TEST_ADC_TRIM5_ADDR                                     ( TEST_BASE_ADDR + TEST_ADC_TRIM5_OFFSET)
#define TEST_ADC_TRIM6_ADDR                                     ( TEST_BASE_ADDR + TEST_ADC_TRIM6_OFFSET)
#define TEST_ADC_TRIM7_ADDR                                     ( TEST_BASE_ADDR + TEST_ADC_TRIM7_OFFSET)
#define TEST_TEST_LOCK_ADDR                                     ( TEST_BASE_ADDR + TEST_TEST_LOCK_OFFSET)

// Register Field Macro Definition
#define TEST_TEST_MODE_CTRL_TM_DET_EN_SHIFT                                       0
#define TEST_TEST_MODE_CTRL_TM_DET_EN_MASK                                        0x00000001
#define TEST_TEST_MODE_CTRL_TM_DET_EN_SET(n)                                      (((uint32_t)(n) << 0  ) & 0x00000001)
#define TEST_TEST_MODE_CTRL_TM_DET_EN_CLR                                         0xFFFFFFFE

#define TEST_TEST_SWD_CTRL_CFG_DIS_SWD_SHIFT                                      0
#define TEST_TEST_SWD_CTRL_CFG_DIS_SWD_MASK                                       0x00000001
#define TEST_TEST_SWD_CTRL_CFG_DIS_SWD_SET(n)                                     (((uint32_t)(n) << 0  ) & 0x00000001)
#define TEST_TEST_SWD_CTRL_CFG_DIS_SWD_CLR                                        0xFFFFFFFE

#define TEST_TEST_PMU_CTRL_SW_SLP_REQ_SHIFT                                       0
#define TEST_TEST_PMU_CTRL_SW_SLP_REQ_MASK                                        0x00000001
#define TEST_TEST_PMU_CTRL_SW_SLP_REQ_SET(n)                                      (((uint32_t)(n) << 0  ) & 0x00000001)
#define TEST_TEST_PMU_CTRL_SW_SLP_REQ_CLR                                         0xFFFFFFFE

#define TEST_TEST_HRC48M_TRIM_HRCS48M_LDO_TRIM_SHIFT                              20
#define TEST_TEST_HRC48M_TRIM_HRCS48M_LDO_TRIM_MASK                               0x00300000
#define TEST_TEST_HRC48M_TRIM_HRCS48M_LDO_TRIM_SET(n)                             (((uint32_t)(n) << 20 ) & 0x00300000)
#define TEST_TEST_HRC48M_TRIM_HRCS48M_LDO_TRIM_CLR                                0xFFCFFFFF

#define TEST_TEST_HRC48M_TRIM_HRCS48M_I_TRIM_SHIFT                                8
#define TEST_TEST_HRC48M_TRIM_HRCS48M_I_TRIM_MASK                                 0x0003FF00
#define TEST_TEST_HRC48M_TRIM_HRCS48M_I_TRIM_SET(n)                               (((uint32_t)(n) << 8  ) & 0x0003FF00)
#define TEST_TEST_HRC48M_TRIM_HRCS48M_I_TRIM_CLR                                  0xFFFC00FF

#define TEST_TEST_HRC48M_TRIM_HRCS48M_TP_TRIM_SHIFT                               0
#define TEST_TEST_HRC48M_TRIM_HRCS48M_TP_TRIM_MASK                                0x0000003F
#define TEST_TEST_HRC48M_TRIM_HRCS48M_TP_TRIM_SET(n)                              (((uint32_t)(n) << 0  ) & 0x0000003F)
#define TEST_TEST_HRC48M_TRIM_HRCS48M_TP_TRIM_CLR                                 0xFFFFFFC0

#define TEST_TEST_LRC32K_TRIM_LRC32K_RES_TRIM_SHIFT                               4
#define TEST_TEST_LRC32K_TRIM_LRC32K_RES_TRIM_MASK                                0x000000F0
#define TEST_TEST_LRC32K_TRIM_LRC32K_RES_TRIM_SET(n)                              (((uint32_t)(n) << 4  ) & 0x000000F0)
#define TEST_TEST_LRC32K_TRIM_LRC32K_RES_TRIM_CLR                                 0xFFFFFF0F

#define TEST_TEST_LRC32K_TRIM_LRC32K_CAP_TRIM_SHIFT                               0
#define TEST_TEST_LRC32K_TRIM_LRC32K_CAP_TRIM_MASK                                0x00000003
#define TEST_TEST_LRC32K_TRIM_LRC32K_CAP_TRIM_SET(n)                              (((uint32_t)(n) << 0  ) & 0x00000003)
#define TEST_TEST_LRC32K_TRIM_LRC32K_CAP_TRIM_CLR                                 0xFFFFFFFC

#define TEST_TEST_LED_TRIM_LED_LC2_IOUT_TRIM_SHIFT                                24
#define TEST_TEST_LED_TRIM_LED_LC2_IOUT_TRIM_MASK                                 0xFF000000
#define TEST_TEST_LED_TRIM_LED_LC2_IOUT_TRIM_SET(n)                               (((uint32_t)(n) << 24 ) & 0xFF000000)
#define TEST_TEST_LED_TRIM_LED_LC2_IOUT_TRIM_CLR                                  0x00FFFFFF

#define TEST_TEST_LED_TRIM_LED_LC1_IOUT_TRIM_SHIFT                                16
#define TEST_TEST_LED_TRIM_LED_LC1_IOUT_TRIM_MASK                                 0x00FF0000
#define TEST_TEST_LED_TRIM_LED_LC1_IOUT_TRIM_SET(n)                               (((uint32_t)(n) << 16 ) & 0x00FF0000)
#define TEST_TEST_LED_TRIM_LED_LC1_IOUT_TRIM_CLR                                  0xFF00FFFF

#define TEST_TEST_LED_TRIM_LED_LC0_IOUT_TRIM_SHIFT                                8
#define TEST_TEST_LED_TRIM_LED_LC0_IOUT_TRIM_MASK                                 0x0000FF00
#define TEST_TEST_LED_TRIM_LED_LC0_IOUT_TRIM_SET(n)                               (((uint32_t)(n) << 8  ) & 0x0000FF00)
#define TEST_TEST_LED_TRIM_LED_LC0_IOUT_TRIM_CLR                                  0xFFFF00FF

#define TEST_TEST_LED_TRIM_LED_IBIAS_TRIM_SHIFT                                   0
#define TEST_TEST_LED_TRIM_LED_IBIAS_TRIM_MASK                                    0x000000FF
#define TEST_TEST_LED_TRIM_LED_IBIAS_TRIM_SET(n)                                  (((uint32_t)(n) << 0  ) & 0x000000FF)
#define TEST_TEST_LED_TRIM_LED_IBIAS_TRIM_CLR                                     0xFFFFFF00

#define TEST_TEST_ANA_CTRL_TEST_AON_SHIFT                                         13
#define TEST_TEST_ANA_CTRL_TEST_AON_MASK                                          0x00002000
#define TEST_TEST_ANA_CTRL_TEST_AON_SET(n)                                        (((uint32_t)(n) << 13 ) & 0x00002000)
#define TEST_TEST_ANA_CTRL_TEST_AON_CLR                                           0xFFFFDFFF

#define TEST_TEST_ANA_CTRL_TESTMUX_SEL_BUF_SHIFT                                  12
#define TEST_TEST_ANA_CTRL_TESTMUX_SEL_BUF_MASK                                   0x00001000
#define TEST_TEST_ANA_CTRL_TESTMUX_SEL_BUF_SET(n)                                 (((uint32_t)(n) << 12 ) & 0x00001000)
#define TEST_TEST_ANA_CTRL_TESTMUX_SEL_BUF_CLR                                    0xFFFFEFFF

#define TEST_TEST_ANA_CTRL_TESTMUX_TAO_SEL_SHIFT                                  8
#define TEST_TEST_ANA_CTRL_TESTMUX_TAO_SEL_MASK                                   0x00000F00
#define TEST_TEST_ANA_CTRL_TESTMUX_TAO_SEL_SET(n)                                 (((uint32_t)(n) << 8  ) & 0x00000F00)
#define TEST_TEST_ANA_CTRL_TESTMUX_TAO_SEL_CLR                                    0xFFFFF0FF

#define TEST_TEST_ANA_CTRL_TESTMUX_TAI_SEL_SHIFT                                  4
#define TEST_TEST_ANA_CTRL_TESTMUX_TAI_SEL_MASK                                   0x000000F0
#define TEST_TEST_ANA_CTRL_TESTMUX_TAI_SEL_SET(n)                                 (((uint32_t)(n) << 4  ) & 0x000000F0)
#define TEST_TEST_ANA_CTRL_TESTMUX_TAI_SEL_CLR                                    0xFFFFFF0F

#define TEST_TEST_ANA_CTRL_TESTMUX_TO_ADC_EN_SHIFT                                3
#define TEST_TEST_ANA_CTRL_TESTMUX_TO_ADC_EN_MASK                                 0x00000008
#define TEST_TEST_ANA_CTRL_TESTMUX_TO_ADC_EN_SET(n)                               (((uint32_t)(n) << 3  ) & 0x00000008)
#define TEST_TEST_ANA_CTRL_TESTMUX_TO_ADC_EN_CLR                                  0xFFFFFFF7

#define TEST_TEST_ANA_CTRL_TESTMUX_TAO_EN_SHIFT                                   2
#define TEST_TEST_ANA_CTRL_TESTMUX_TAO_EN_MASK                                    0x00000004
#define TEST_TEST_ANA_CTRL_TESTMUX_TAO_EN_SET(n)                                  (((uint32_t)(n) << 2  ) & 0x00000004)
#define TEST_TEST_ANA_CTRL_TESTMUX_TAO_EN_CLR                                     0xFFFFFFFB

#define TEST_TEST_ANA_CTRL_TESTMUX_TAI_EN_SHIFT                                   1
#define TEST_TEST_ANA_CTRL_TESTMUX_TAI_EN_MASK                                    0x00000002
#define TEST_TEST_ANA_CTRL_TESTMUX_TAI_EN_SET(n)                                  (((uint32_t)(n) << 1  ) & 0x00000002)
#define TEST_TEST_ANA_CTRL_TESTMUX_TAI_EN_CLR                                     0xFFFFFFFD

#define TEST_TEST_ANA_CTRL_TESTMUX_BUF_EN_SHIFT                                   0
#define TEST_TEST_ANA_CTRL_TESTMUX_BUF_EN_MASK                                    0x00000001
#define TEST_TEST_ANA_CTRL_TESTMUX_BUF_EN_SET(n)                                  (((uint32_t)(n) << 0  ) & 0x00000001)
#define TEST_TEST_ANA_CTRL_TESTMUX_BUF_EN_CLR                                     0xFFFFFFFE

#define TEST_TEST_TPREF_TRIM_TPREF_TRIM_SHIFT                                     0
#define TEST_TEST_TPREF_TRIM_TPREF_TRIM_MASK                                      0x0000007F
#define TEST_TEST_TPREF_TRIM_TPREF_TRIM_SET(n)                                    (((uint32_t)(n) << 0  ) & 0x0000007F)
#define TEST_TEST_TPREF_TRIM_TPREF_TRIM_CLR                                       0xFFFFFF80

#define TEST_TEST_LDO33_TRIM_LDO33_PORB_SEL_SHIFT                                 4
#define TEST_TEST_LDO33_TRIM_LDO33_PORB_SEL_MASK                                  0x00000030
#define TEST_TEST_LDO33_TRIM_LDO33_PORB_SEL_SET(n)                                (((uint32_t)(n) << 4  ) & 0x00000030)
#define TEST_TEST_LDO33_TRIM_LDO33_PORB_SEL_CLR                                   0xFFFFFFCF

#define TEST_TEST_LDO33_TRIM_LDO33_TRIM_SHIFT                                     0
#define TEST_TEST_LDO33_TRIM_LDO33_TRIM_MASK                                      0x00000007
#define TEST_TEST_LDO33_TRIM_LDO33_TRIM_SET(n)                                    (((uint32_t)(n) << 0  ) & 0x00000007)
#define TEST_TEST_LDO33_TRIM_LDO33_TRIM_CLR                                       0xFFFFFFF8

#define TEST_TEST_OTP_TRIM_OTP_TRIM_SHIFT                                         0
#define TEST_TEST_OTP_TRIM_OTP_TRIM_MASK                                          0x0000001F
#define TEST_TEST_OTP_TRIM_OTP_TRIM_SET(n)                                        (((uint32_t)(n) << 0  ) & 0x0000001F)
#define TEST_TEST_OTP_TRIM_OTP_TRIM_CLR                                           0xFFFFFFE0

#define TEST_TEST_LIN_TRIM_LINAA_ITRIM_SHIFT                                      4
#define TEST_TEST_LIN_TRIM_LINAA_ITRIM_MASK                                       0x000000F0
#define TEST_TEST_LIN_TRIM_LINAA_ITRIM_SET(n)                                     (((uint32_t)(n) << 4  ) & 0x000000F0)
#define TEST_TEST_LIN_TRIM_LINAA_ITRIM_CLR                                        0xFFFFFF0F

#define TEST_TEST_LIN_TRIM_LIN_TRIM_BUSLIM_SHIFT                                  0
#define TEST_TEST_LIN_TRIM_LIN_TRIM_BUSLIM_MASK                                   0x0000000F
#define TEST_TEST_LIN_TRIM_LIN_TRIM_BUSLIM_SET(n)                                 (((uint32_t)(n) << 0  ) & 0x0000000F)
#define TEST_TEST_LIN_TRIM_LIN_TRIM_BUSLIM_CLR                                    0xFFFFFFF0

#define TEST_TEST_LIN_CTRL_LINAA_TESTSEL_SHIFT                                    28
#define TEST_TEST_LIN_CTRL_LINAA_TESTSEL_MASK                                     0x70000000
#define TEST_TEST_LIN_CTRL_LINAA_TESTSEL_SET(n)                                   (((uint32_t)(n) << 28 ) & 0x70000000)
#define TEST_TEST_LIN_CTRL_LINAA_TESTSEL_CLR                                      0x8FFFFFFF

#define TEST_TEST_LIN_CTRL_LIN_HARD_SWITCH_SHIFT                                  25
#define TEST_TEST_LIN_CTRL_LIN_HARD_SWITCH_MASK                                   0x02000000
#define TEST_TEST_LIN_CTRL_LIN_HARD_SWITCH_SET(n)                                 (((uint32_t)(n) << 25 ) & 0x02000000)
#define TEST_TEST_LIN_CTRL_LIN_HARD_SWITCH_CLR                                    0xFDFFFFFF

#define TEST_TEST_LIN_CTRL_LIN_PDSSOK_EN_SHIFT                                    24
#define TEST_TEST_LIN_CTRL_LIN_PDSSOK_EN_MASK                                     0x01000000
#define TEST_TEST_LIN_CTRL_LIN_PDSSOK_EN_SET(n)                                   (((uint32_t)(n) << 24 ) & 0x01000000)
#define TEST_TEST_LIN_CTRL_LIN_PDSSOK_EN_CLR                                      0xFEFFFFFF

#define TEST_TEST_LIN_CTRL_LIN_EMC_PU_CUR_SHIFT                                   20
#define TEST_TEST_LIN_CTRL_LIN_EMC_PU_CUR_MASK                                    0x00700000
#define TEST_TEST_LIN_CTRL_LIN_EMC_PU_CUR_SET(n)                                  (((uint32_t)(n) << 20 ) & 0x00700000)
#define TEST_TEST_LIN_CTRL_LIN_EMC_PU_CUR_CLR                                     0xFF8FFFFF

#define TEST_TEST_LIN_CTRL_LIN_EMC_PD_CUR_SHIFT                                   16
#define TEST_TEST_LIN_CTRL_LIN_EMC_PD_CUR_MASK                                    0x00070000
#define TEST_TEST_LIN_CTRL_LIN_EMC_PD_CUR_SET(n)                                  (((uint32_t)(n) << 16 ) & 0x00070000)
#define TEST_TEST_LIN_CTRL_LIN_EMC_PD_CUR_CLR                                     0xFFF8FFFF

#define TEST_TEST_LIN_CTRL_LIN_RX_DELAY_SHIFT                                     12
#define TEST_TEST_LIN_CTRL_LIN_RX_DELAY_MASK                                      0x0000F000
#define TEST_TEST_LIN_CTRL_LIN_RX_DELAY_SET(n)                                    (((uint32_t)(n) << 12 ) & 0x0000F000)
#define TEST_TEST_LIN_CTRL_LIN_RX_DELAY_CLR                                       0xFFFF0FFF

#define TEST_TEST_LIN_CTRL_LIN_EMC_LEVEL_SHIFT                                    8
#define TEST_TEST_LIN_CTRL_LIN_EMC_LEVEL_MASK                                     0x00000F00
#define TEST_TEST_LIN_CTRL_LIN_EMC_LEVEL_SET(n)                                   (((uint32_t)(n) << 8  ) & 0x00000F00)
#define TEST_TEST_LIN_CTRL_LIN_EMC_LEVEL_CLR                                      0xFFFFF0FF

#define TEST_TEST_LIN_CTRL_LIN_EMC_FBNEG_EN_SHIFT                                 6
#define TEST_TEST_LIN_CTRL_LIN_EMC_FBNEG_EN_MASK                                  0x00000040
#define TEST_TEST_LIN_CTRL_LIN_EMC_FBNEG_EN_SET(n)                                (((uint32_t)(n) << 6  ) & 0x00000040)
#define TEST_TEST_LIN_CTRL_LIN_EMC_FBNEG_EN_CLR                                   0xFFFFFFBF

#define TEST_TEST_LIN_CTRL_LIN_EMC_SLOPEDIS_PU_EN_SHIFT                           5
#define TEST_TEST_LIN_CTRL_LIN_EMC_SLOPEDIS_PU_EN_MASK                            0x00000020
#define TEST_TEST_LIN_CTRL_LIN_EMC_SLOPEDIS_PU_EN_SET(n)                          (((uint32_t)(n) << 5  ) & 0x00000020)
#define TEST_TEST_LIN_CTRL_LIN_EMC_SLOPEDIS_PU_EN_CLR                             0xFFFFFFDF

#define TEST_TEST_LIN_CTRL_LIN_EMC_SLOPEDIS_PD_EN_SHIFT                           4
#define TEST_TEST_LIN_CTRL_LIN_EMC_SLOPEDIS_PD_EN_MASK                            0x00000010
#define TEST_TEST_LIN_CTRL_LIN_EMC_SLOPEDIS_PD_EN_SET(n)                          (((uint32_t)(n) << 4  ) & 0x00000010)
#define TEST_TEST_LIN_CTRL_LIN_EMC_SLOPEDIS_PD_EN_CLR                             0xFFFFFFEF

#define TEST_TEST_LIN_CTRL_LIN_EMC_PU_EN_SHIFT                                    3
#define TEST_TEST_LIN_CTRL_LIN_EMC_PU_EN_MASK                                     0x00000008
#define TEST_TEST_LIN_CTRL_LIN_EMC_PU_EN_SET(n)                                   (((uint32_t)(n) << 3  ) & 0x00000008)
#define TEST_TEST_LIN_CTRL_LIN_EMC_PU_EN_CLR                                      0xFFFFFFF7

#define TEST_TEST_LIN_CTRL_LIN_EMC_PD_EN_SHIFT                                    2
#define TEST_TEST_LIN_CTRL_LIN_EMC_PD_EN_MASK                                     0x00000004
#define TEST_TEST_LIN_CTRL_LIN_EMC_PD_EN_SET(n)                                   (((uint32_t)(n) << 2  ) & 0x00000004)
#define TEST_TEST_LIN_CTRL_LIN_EMC_PD_EN_CLR                                      0xFFFFFFFB

#define TEST_TEST_LIN_CTRL_LIN_EMC_EN_SHIFT                                       1
#define TEST_TEST_LIN_CTRL_LIN_EMC_EN_MASK                                        0x00000002
#define TEST_TEST_LIN_CTRL_LIN_EMC_EN_SET(n)                                      (((uint32_t)(n) << 1  ) & 0x00000002)
#define TEST_TEST_LIN_CTRL_LIN_EMC_EN_CLR                                         0xFFFFFFFD

#define TEST_TEST_LIN_CTRL_LIN_TM_RX_SHIFT                                        0
#define TEST_TEST_LIN_CTRL_LIN_TM_RX_MASK                                         0x00000001
#define TEST_TEST_LIN_CTRL_LIN_TM_RX_SET(n)                                       (((uint32_t)(n) << 0  ) & 0x00000001)
#define TEST_TEST_LIN_CTRL_LIN_TM_RX_CLR                                          0xFFFFFFFE

#define TEST_TEST_LIN_STATUS_LIN_SLOPE_OK_SHIFT                                   2
#define TEST_TEST_LIN_STATUS_LIN_SLOPE_OK_MASK                                    0x00000004
#define TEST_TEST_LIN_STATUS_LIN_SLOPE_OK_SET(n)                                  (((uint32_t)(n) << 2  ) & 0x00000004)
#define TEST_TEST_LIN_STATUS_LIN_SLOPE_OK_CLR                                     0xFFFFFFFB

#define TEST_TEST_LIN_STATUS_LIN_COMPOUT_SHIFT                                    1
#define TEST_TEST_LIN_STATUS_LIN_COMPOUT_MASK                                     0x00000002
#define TEST_TEST_LIN_STATUS_LIN_COMPOUT_SET(n)                                   (((uint32_t)(n) << 1  ) & 0x00000002)
#define TEST_TEST_LIN_STATUS_LIN_COMPOUT_CLR                                      0xFFFFFFFD

#define TEST_TEST_LIN_STATUS_LIN_SLOPEDIS_SHIFT                                   0
#define TEST_TEST_LIN_STATUS_LIN_SLOPEDIS_MASK                                    0x00000001
#define TEST_TEST_LIN_STATUS_LIN_SLOPEDIS_SET(n)                                  (((uint32_t)(n) << 0  ) & 0x00000001)
#define TEST_TEST_LIN_STATUS_LIN_SLOPEDIS_CLR                                     0xFFFFFFFE

#define TEST_TEST_ADC_CTRL_ADC_EXT_CLK_EN_SHIFT                                   8
#define TEST_TEST_ADC_CTRL_ADC_EXT_CLK_EN_MASK                                    0x00000100
#define TEST_TEST_ADC_CTRL_ADC_EXT_CLK_EN_SET(n)                                  (((uint32_t)(n) << 8  ) & 0x00000100)
#define TEST_TEST_ADC_CTRL_ADC_EXT_CLK_EN_CLR                                     0xFFFFFEFF

#define TEST_TEST_ADC_CTRL_ADC_TEST_SEL_SHIFT                                     4
#define TEST_TEST_ADC_CTRL_ADC_TEST_SEL_MASK                                      0x00000070
#define TEST_TEST_ADC_CTRL_ADC_TEST_SEL_SET(n)                                    (((uint32_t)(n) << 4  ) & 0x00000070)
#define TEST_TEST_ADC_CTRL_ADC_TEST_SEL_CLR                                       0xFFFFFF8F

#define TEST_TEST_ADC_CTRL_ADC_VCM_TEST_SEL_SHIFT                                 2
#define TEST_TEST_ADC_CTRL_ADC_VCM_TEST_SEL_MASK                                  0x00000004
#define TEST_TEST_ADC_CTRL_ADC_VCM_TEST_SEL_SET(n)                                (((uint32_t)(n) << 2  ) & 0x00000004)
#define TEST_TEST_ADC_CTRL_ADC_VCM_TEST_SEL_CLR                                   0xFFFFFFFB

#define TEST_TEST_ADC_CTRL_ADC_VREF_TEST_SEL_SHIFT                                0
#define TEST_TEST_ADC_CTRL_ADC_VREF_TEST_SEL_MASK                                 0x00000003
#define TEST_TEST_ADC_CTRL_ADC_VREF_TEST_SEL_SET(n)                               (((uint32_t)(n) << 0  ) & 0x00000003)
#define TEST_TEST_ADC_CTRL_ADC_VREF_TEST_SEL_CLR                                  0xFFFFFFFC

#define TEST_TEST_BUS_CTRL_IP_TP_SEL_SHIFT                                        12
#define TEST_TEST_BUS_CTRL_IP_TP_SEL_MASK                                         0x00007000
#define TEST_TEST_BUS_CTRL_IP_TP_SEL_SET(n)                                       (((uint32_t)(n) << 12 ) & 0x00007000)
#define TEST_TEST_BUS_CTRL_IP_TP_SEL_CLR                                          0xFFFF8FFF

#define TEST_TEST_BUS_CTRL_TOP_TP_OUT_SEL_SHIFT                                   10
#define TEST_TEST_BUS_CTRL_TOP_TP_OUT_SEL_MASK                                    0x00000C00
#define TEST_TEST_BUS_CTRL_TOP_TP_OUT_SEL_SET(n)                                  (((uint32_t)(n) << 10 ) & 0x00000C00)
#define TEST_TEST_BUS_CTRL_TOP_TP_OUT_SEL_CLR                                     0xFFFFF3FF

#define TEST_TEST_BUS_CTRL_TOP_TP_HNIBBLE_SEL_SHIFT                               9
#define TEST_TEST_BUS_CTRL_TOP_TP_HNIBBLE_SEL_MASK                                0x00000200
#define TEST_TEST_BUS_CTRL_TOP_TP_HNIBBLE_SEL_SET(n)                              (((uint32_t)(n) << 9  ) & 0x00000200)
#define TEST_TEST_BUS_CTRL_TOP_TP_HNIBBLE_SEL_CLR                                 0xFFFFFDFF

#define TEST_TEST_BUS_CTRL_TOP_TP_HBYTE_SEL_SHIFT                                 8
#define TEST_TEST_BUS_CTRL_TOP_TP_HBYTE_SEL_MASK                                  0x00000100
#define TEST_TEST_BUS_CTRL_TOP_TP_HBYTE_SEL_SET(n)                                (((uint32_t)(n) << 8  ) & 0x00000100)
#define TEST_TEST_BUS_CTRL_TOP_TP_HBYTE_SEL_CLR                                   0xFFFFFEFF

#define TEST_TEST_BUS_CTRL_TOP_TP_IP_SEL_SHIFT                                    4
#define TEST_TEST_BUS_CTRL_TOP_TP_IP_SEL_MASK                                     0x000000F0
#define TEST_TEST_BUS_CTRL_TOP_TP_IP_SEL_SET(n)                                   (((uint32_t)(n) << 4  ) & 0x000000F0)
#define TEST_TEST_BUS_CTRL_TOP_TP_IP_SEL_CLR                                      0xFFFFFF0F

#define TEST_TEST_BUS_CTRL_TOP_TP_EN_SHIFT                                        0
#define TEST_TEST_BUS_CTRL_TOP_TP_EN_MASK                                         0x00000001
#define TEST_TEST_BUS_CTRL_TOP_TP_EN_SET(n)                                       (((uint32_t)(n) << 0  ) & 0x00000001)
#define TEST_TEST_BUS_CTRL_TOP_TP_EN_CLR                                          0xFFFFFFFE

#define TEST_TEST_LDO15_TRIM_LDO15_PORB_SEL_SHIFT                                 4
#define TEST_TEST_LDO15_TRIM_LDO15_PORB_SEL_MASK                                  0x00000030
#define TEST_TEST_LDO15_TRIM_LDO15_PORB_SEL_SET(n)                                (((uint32_t)(n) << 4  ) & 0x00000030)
#define TEST_TEST_LDO15_TRIM_LDO15_PORB_SEL_CLR                                   0xFFFFFFCF

#define TEST_TEST_LDO15_TRIM_LDO15_TRIM_SHIFT                                     0
#define TEST_TEST_LDO15_TRIM_LDO15_TRIM_MASK                                      0x00000007
#define TEST_TEST_LDO15_TRIM_LDO15_TRIM_SET(n)                                    (((uint32_t)(n) << 0  ) & 0x00000007)
#define TEST_TEST_LDO15_TRIM_LDO15_TRIM_CLR                                       0xFFFFFFF8

#define TEST_TM_STATUS_NVR_DIS_TM_ALL_SHIFT                                       1
#define TEST_TM_STATUS_NVR_DIS_TM_ALL_MASK                                        0x00000002
#define TEST_TM_STATUS_NVR_DIS_TM_ALL_SET(n)                                      (((uint32_t)(n) << 1  ) & 0x00000002)
#define TEST_TM_STATUS_NVR_DIS_TM_ALL_CLR                                         0xFFFFFFFD

#define TEST_TM_STATUS_NVR_DIS_TM_SHIFT                                           0
#define TEST_TM_STATUS_NVR_DIS_TM_MASK                                            0x00000001
#define TEST_TM_STATUS_NVR_DIS_TM_SET(n)                                          (((uint32_t)(n) << 0  ) & 0x00000001)
#define TEST_TM_STATUS_NVR_DIS_TM_CLR                                             0xFFFFFFFE

#define TEST_TEST_LED_IB_DIAG_TRIM_LED_IB_DIAG2_TRIM_SHIFT                        16
#define TEST_TEST_LED_IB_DIAG_TRIM_LED_IB_DIAG2_TRIM_MASK                         0x00FF0000
#define TEST_TEST_LED_IB_DIAG_TRIM_LED_IB_DIAG2_TRIM_SET(n)                       (((uint32_t)(n) << 16 ) & 0x00FF0000)
#define TEST_TEST_LED_IB_DIAG_TRIM_LED_IB_DIAG2_TRIM_CLR                          0xFF00FFFF

#define TEST_TEST_LED_IB_DIAG_TRIM_LED_IB_DIAG1_TRIM_SHIFT                        8
#define TEST_TEST_LED_IB_DIAG_TRIM_LED_IB_DIAG1_TRIM_MASK                         0x0000FF00
#define TEST_TEST_LED_IB_DIAG_TRIM_LED_IB_DIAG1_TRIM_SET(n)                       (((uint32_t)(n) << 8  ) & 0x0000FF00)
#define TEST_TEST_LED_IB_DIAG_TRIM_LED_IB_DIAG1_TRIM_CLR                          0xFFFF00FF

#define TEST_TEST_LED_IB_DIAG_TRIM_LED_IB_DIAG0_TRIM_SHIFT                        0
#define TEST_TEST_LED_IB_DIAG_TRIM_LED_IB_DIAG0_TRIM_MASK                         0x000000FF
#define TEST_TEST_LED_IB_DIAG_TRIM_LED_IB_DIAG0_TRIM_SET(n)                       (((uint32_t)(n) << 0  ) & 0x000000FF)
#define TEST_TEST_LED_IB_DIAG_TRIM_LED_IB_DIAG0_TRIM_CLR                          0xFFFFFF00

#define TEST_TEST_BIAS_TRIM_BIAS_TRIM_SHIFT                                       0
#define TEST_TEST_BIAS_TRIM_BIAS_TRIM_MASK                                        0x00000007
#define TEST_TEST_BIAS_TRIM_BIAS_TRIM_SET(n)                                      (((uint32_t)(n) << 0  ) & 0x00000007)
#define TEST_TEST_BIAS_TRIM_BIAS_TRIM_CLR                                         0xFFFFFFF8

#define TEST_TEST_LED_LDO5_TRIM_LED_LDO5_OCP_EN_SHIFT                             16
#define TEST_TEST_LED_LDO5_TRIM_LED_LDO5_OCP_EN_MASK                              0x00010000
#define TEST_TEST_LED_LDO5_TRIM_LED_LDO5_OCP_EN_SET(n)                            (((uint32_t)(n) << 16 ) & 0x00010000)
#define TEST_TEST_LED_LDO5_TRIM_LED_LDO5_OCP_EN_CLR                               0xFFFEFFFF

#define TEST_TEST_LED_LDO5_TRIM_LED_RES_TC_TRIM_SHIFT                             8
#define TEST_TEST_LED_LDO5_TRIM_LED_RES_TC_TRIM_MASK                              0x00001F00
#define TEST_TEST_LED_LDO5_TRIM_LED_RES_TC_TRIM_SET(n)                            (((uint32_t)(n) << 8  ) & 0x00001F00)
#define TEST_TEST_LED_LDO5_TRIM_LED_RES_TC_TRIM_CLR                               0xFFFFE0FF

#define TEST_TEST_LED_LDO5_TRIM_LED_LDO5_OUT_TRIM_SHIFT                           4
#define TEST_TEST_LED_LDO5_TRIM_LED_LDO5_OUT_TRIM_MASK                            0x000000F0
#define TEST_TEST_LED_LDO5_TRIM_LED_LDO5_OUT_TRIM_SET(n)                          (((uint32_t)(n) << 4  ) & 0x000000F0)
#define TEST_TEST_LED_LDO5_TRIM_LED_LDO5_OUT_TRIM_CLR                             0xFFFFFF0F

#define TEST_TEST_LED_LDO5_TRIM_LED_LDO5_OCP_TRIM_SHIFT                           0
#define TEST_TEST_LED_LDO5_TRIM_LED_LDO5_OCP_TRIM_MASK                            0x00000003
#define TEST_TEST_LED_LDO5_TRIM_LED_LDO5_OCP_TRIM_SET(n)                          (((uint32_t)(n) << 0  ) & 0x00000003)
#define TEST_TEST_LED_LDO5_TRIM_LED_LDO5_OCP_TRIM_CLR                             0xFFFFFFFC

#define TEST_TEST_RESERVED_T_TRIM_RESERVED_T_IDAC_TRIM_SHIFT                      0
#define TEST_TEST_RESERVED_T_TRIM_RESERVED_T_IDAC_TRIM_MASK                       0x0000000F
#define TEST_TEST_RESERVED_T_TRIM_RESERVED_T_IDAC_TRIM_SET(n)                     (((uint32_t)(n) << 0  ) & 0x0000000F)
#define TEST_TEST_RESERVED_T_TRIM_RESERVED_T_IDAC_TRIM_CLR                        0xFFFFFFF0

#define TEST_ADC_TRIM0_SARADC_TRIM_VAL3_SHIFT                                     24
#define TEST_ADC_TRIM0_SARADC_TRIM_VAL3_MASK                                      0xFF000000
#define TEST_ADC_TRIM0_SARADC_TRIM_VAL3_SET(n)                                    (((uint32_t)(n) << 24 ) & 0xFF000000)
#define TEST_ADC_TRIM0_SARADC_TRIM_VAL3_CLR                                       0x00FFFFFF

#define TEST_ADC_TRIM0_SARADC_TRIM_VAL2_SHIFT                                     16
#define TEST_ADC_TRIM0_SARADC_TRIM_VAL2_MASK                                      0x007F0000
#define TEST_ADC_TRIM0_SARADC_TRIM_VAL2_SET(n)                                    (((uint32_t)(n) << 16 ) & 0x007F0000)
#define TEST_ADC_TRIM0_SARADC_TRIM_VAL2_CLR                                       0xFF80FFFF

#define TEST_ADC_TRIM0_SARADC_TRIM_VAL1_SHIFT                                     8
#define TEST_ADC_TRIM0_SARADC_TRIM_VAL1_MASK                                      0x00003F00
#define TEST_ADC_TRIM0_SARADC_TRIM_VAL1_SET(n)                                    (((uint32_t)(n) << 8  ) & 0x00003F00)
#define TEST_ADC_TRIM0_SARADC_TRIM_VAL1_CLR                                       0xFFFFC0FF

#define TEST_ADC_TRIM0_SARADC_TRIM_VAL0_SHIFT                                     0
#define TEST_ADC_TRIM0_SARADC_TRIM_VAL0_MASK                                      0x0000003F
#define TEST_ADC_TRIM0_SARADC_TRIM_VAL0_SET(n)                                    (((uint32_t)(n) << 0  ) & 0x0000003F)
#define TEST_ADC_TRIM0_SARADC_TRIM_VAL0_CLR                                       0xFFFFFFC0

#define TEST_ADC_TRIM1_SARADC_TRIM_VAL5_SHIFT                                     16
#define TEST_ADC_TRIM1_SARADC_TRIM_VAL5_MASK                                      0x01FF0000
#define TEST_ADC_TRIM1_SARADC_TRIM_VAL5_SET(n)                                    (((uint32_t)(n) << 16 ) & 0x01FF0000)
#define TEST_ADC_TRIM1_SARADC_TRIM_VAL5_CLR                                       0xFE00FFFF

#define TEST_ADC_TRIM1_SARADC_TRIM_VAL4_SHIFT                                     0
#define TEST_ADC_TRIM1_SARADC_TRIM_VAL4_MASK                                      0x000000FF
#define TEST_ADC_TRIM1_SARADC_TRIM_VAL4_SET(n)                                    (((uint32_t)(n) << 0  ) & 0x000000FF)
#define TEST_ADC_TRIM1_SARADC_TRIM_VAL4_CLR                                       0xFFFFFF00

#define TEST_ADC_TRIM2_SARADC_TRIM_VAL7_SHIFT                                     16
#define TEST_ADC_TRIM2_SARADC_TRIM_VAL7_MASK                                      0x03FF0000
#define TEST_ADC_TRIM2_SARADC_TRIM_VAL7_SET(n)                                    (((uint32_t)(n) << 16 ) & 0x03FF0000)
#define TEST_ADC_TRIM2_SARADC_TRIM_VAL7_CLR                                       0xFC00FFFF

#define TEST_ADC_TRIM2_SARADC_TRIM_VAL6_SHIFT                                     0
#define TEST_ADC_TRIM2_SARADC_TRIM_VAL6_MASK                                      0x000001FF
#define TEST_ADC_TRIM2_SARADC_TRIM_VAL6_SET(n)                                    (((uint32_t)(n) << 0  ) & 0x000001FF)
#define TEST_ADC_TRIM2_SARADC_TRIM_VAL6_CLR                                       0xFFFFFE00

#define TEST_ADC_TRIM3_SARADC_TRIM_VAL9_SHIFT                                     16
#define TEST_ADC_TRIM3_SARADC_TRIM_VAL9_MASK                                      0x0FFF0000
#define TEST_ADC_TRIM3_SARADC_TRIM_VAL9_SET(n)                                    (((uint32_t)(n) << 16 ) & 0x0FFF0000)
#define TEST_ADC_TRIM3_SARADC_TRIM_VAL9_CLR                                       0xF000FFFF

#define TEST_ADC_TRIM3_SARADC_TRIM_VAL8_SHIFT                                     0
#define TEST_ADC_TRIM3_SARADC_TRIM_VAL8_MASK                                      0x000007FF
#define TEST_ADC_TRIM3_SARADC_TRIM_VAL8_SET(n)                                    (((uint32_t)(n) << 0  ) & 0x000007FF)
#define TEST_ADC_TRIM3_SARADC_TRIM_VAL8_CLR                                       0xFFFFF800

#define TEST_ADC_TRIM4_SARADC_TRIM_VAL11_SHIFT                                    16
#define TEST_ADC_TRIM4_SARADC_TRIM_VAL11_MASK                                     0x1FFF0000
#define TEST_ADC_TRIM4_SARADC_TRIM_VAL11_SET(n)                                   (((uint32_t)(n) << 16 ) & 0x1FFF0000)
#define TEST_ADC_TRIM4_SARADC_TRIM_VAL11_CLR                                      0xE000FFFF

#define TEST_ADC_TRIM4_SARADC_TRIM_VAL10_SHIFT                                    0
#define TEST_ADC_TRIM4_SARADC_TRIM_VAL10_MASK                                     0x00000FFF
#define TEST_ADC_TRIM4_SARADC_TRIM_VAL10_SET(n)                                   (((uint32_t)(n) << 0  ) & 0x00000FFF)
#define TEST_ADC_TRIM4_SARADC_TRIM_VAL10_CLR                                      0xFFFFF000

#define TEST_ADC_TRIM5_SARADC_TRIM_VAL13_SHIFT                                    16
#define TEST_ADC_TRIM5_SARADC_TRIM_VAL13_MASK                                     0x1FFF0000
#define TEST_ADC_TRIM5_SARADC_TRIM_VAL13_SET(n)                                   (((uint32_t)(n) << 16 ) & 0x1FFF0000)
#define TEST_ADC_TRIM5_SARADC_TRIM_VAL13_CLR                                      0xE000FFFF

#define TEST_ADC_TRIM5_SARADC_TRIM_VAL12_SHIFT                                    0
#define TEST_ADC_TRIM5_SARADC_TRIM_VAL12_MASK                                     0x00003FFF
#define TEST_ADC_TRIM5_SARADC_TRIM_VAL12_SET(n)                                   (((uint32_t)(n) << 0  ) & 0x00003FFF)
#define TEST_ADC_TRIM5_SARADC_TRIM_VAL12_CLR                                      0xFFFFC000

#define TEST_ADC_TRIM6_SARADC_TRIM_VAL15_SHIFT                                    16
#define TEST_ADC_TRIM6_SARADC_TRIM_VAL15_MASK                                     0x7FFF0000
#define TEST_ADC_TRIM6_SARADC_TRIM_VAL15_SET(n)                                   (((uint32_t)(n) << 16 ) & 0x7FFF0000)
#define TEST_ADC_TRIM6_SARADC_TRIM_VAL15_CLR                                      0x8000FFFF

#define TEST_ADC_TRIM6_SARADC_TRIM_VAL14_SHIFT                                    0
#define TEST_ADC_TRIM6_SARADC_TRIM_VAL14_MASK                                     0x00003FFF
#define TEST_ADC_TRIM6_SARADC_TRIM_VAL14_SET(n)                                   (((uint32_t)(n) << 0  ) & 0x00003FFF)
#define TEST_ADC_TRIM6_SARADC_TRIM_VAL14_CLR                                      0xFFFFC000

#define TEST_ADC_TRIM7_SARADC_TRIM_VAL16_SHIFT                                    0
#define TEST_ADC_TRIM7_SARADC_TRIM_VAL16_MASK                                     0x0000FFFF
#define TEST_ADC_TRIM7_SARADC_TRIM_VAL16_SET(n)                                   (((uint32_t)(n) << 0  ) & 0x0000FFFF)
#define TEST_ADC_TRIM7_SARADC_TRIM_VAL16_CLR                                      0xFFFF0000

#define TEST_TEST_LOCK_TEST_LOCK_SHIFT                                            0
#define TEST_TEST_LOCK_TEST_LOCK_MASK                                             0x00000001
#define TEST_TEST_LOCK_TEST_LOCK_SET(n)                                           (((uint32_t)(n) << 0  ) & 0x00000001)
#define TEST_TEST_LOCK_TEST_LOCK_CLR                                              0xFFFFFFFE

// Register Structure Definition
typedef struct
{
    __IO uint32_t TM_DET_EN              : 1  ; // 0  : 0
    __IO uint32_t RESERVED0              : 31 ; // 31 : 1
} TEST_TEST_MODE_CTRL_FIELD_T;

typedef struct
{
    __IO uint32_t CFG_DIS_SWD            : 1  ; // 0  : 0
    __IO uint32_t RESERVED0              : 31 ; // 31 : 1
} TEST_TEST_SWD_CTRL_FIELD_T;

typedef struct
{
    __IO uint32_t SW_SLP_REQ             : 1  ; // 0  : 0
    __IO uint32_t RESERVED0              : 31 ; // 31 : 1
} TEST_TEST_PMU_CTRL_FIELD_T;

typedef struct
{
    __IO uint32_t HRCS48M_TP_TRIM        : 6  ; // 5  : 0
    __IO uint32_t RESERVED2              : 2  ; // 7  : 6
    __IO uint32_t HRCS48M_I_TRIM         : 10 ; // 17 : 8
    __IO uint32_t RESERVED1              : 2  ; // 19 : 18
    __IO uint32_t HRCS48M_LDO_TRIM       : 2  ; // 21 : 20
    __IO uint32_t RESERVED0              : 10 ; // 31 : 22
} TEST_TEST_HRC48M_TRIM_FIELD_T;

typedef struct
{
    __IO uint32_t LRC32K_CAP_TRIM        : 2  ; // 1  : 0
    __IO uint32_t RESERVED1              : 2  ; // 3  : 2
    __IO uint32_t LRC32K_RES_TRIM        : 4  ; // 7  : 4
    __IO uint32_t RESERVED0              : 24 ; // 31 : 8
} TEST_TEST_LRC32K_TRIM_FIELD_T;

typedef struct
{
    __IO uint32_t LED_IBIAS_TRIM         : 8  ; // 7  : 0
    __IO uint32_t LED_LC0_IOUT_TRIM      : 8  ; // 15 : 8
    __IO uint32_t LED_LC1_IOUT_TRIM      : 8  ; // 23 : 16
    __IO uint32_t LED_LC2_IOUT_TRIM      : 8  ; // 31 : 24
} TEST_TEST_LED_TRIM_FIELD_T;

typedef struct
{
    __IO uint32_t TESTMUX_BUF_EN         : 1  ; // 0  : 0
    __IO uint32_t TESTMUX_TAI_EN         : 1  ; // 1  : 1
    __IO uint32_t TESTMUX_TAO_EN         : 1  ; // 2  : 2
    __IO uint32_t TESTMUX_TO_ADC_EN      : 1  ; // 3  : 3
    __IO uint32_t TESTMUX_TAI_SEL        : 4  ; // 7  : 4
    __IO uint32_t TESTMUX_TAO_SEL        : 4  ; // 11 : 8
    __IO uint32_t TESTMUX_SEL_BUF        : 1  ; // 12 : 12
    __IO uint32_t TEST_AON               : 1  ; // 13 : 13
    __IO uint32_t RESERVED0              : 18 ; // 31 : 14
} TEST_TEST_ANA_CTRL_FIELD_T;

typedef struct
{
    __IO uint32_t TPREF_TRIM             : 7  ; // 6  : 0
    __IO uint32_t RESERVED0              : 25 ; // 31 : 7
} TEST_TEST_TPREF_TRIM_FIELD_T;

typedef struct
{
    __IO uint32_t LDO33_TRIM             : 3  ; // 2  : 0
    __IO uint32_t RESERVED1              : 1  ; // 3  : 3
    __IO uint32_t LDO33_PORB_SEL         : 2  ; // 5  : 4
    __IO uint32_t RESERVED0              : 26 ; // 31 : 6
} TEST_TEST_LDO33_TRIM_FIELD_T;

typedef struct
{
    __IO uint32_t OTP_TRIM               : 5  ; // 4  : 0
    __IO uint32_t RESERVED0              : 27 ; // 31 : 5
} TEST_TEST_OTP_TRIM_FIELD_T;

typedef struct
{
    __IO uint32_t LIN_TRIM_BUSLIM        : 4  ; // 3  : 0
    __IO uint32_t LINAA_ITRIM            : 4  ; // 7  : 4
    __IO uint32_t RESERVED0              : 24 ; // 31 : 8
} TEST_TEST_LIN_TRIM_FIELD_T;

typedef struct
{
    __IO uint32_t LIN_TM_RX              : 1  ; // 0  : 0
    __IO uint32_t LIN_EMC_EN             : 1  ; // 1  : 1
    __IO uint32_t LIN_EMC_PD_EN          : 1  ; // 2  : 2
    __IO uint32_t LIN_EMC_PU_EN          : 1  ; // 3  : 3
    __IO uint32_t LIN_EMC_SLOPEDIS_PD_EN : 1  ; // 4  : 4
    __IO uint32_t LIN_EMC_SLOPEDIS_PU_EN : 1  ; // 5  : 5
    __IO uint32_t LIN_EMC_FBNEG_EN       : 1  ; // 6  : 6
    __IO uint32_t RESERVED4              : 1  ; // 7  : 7
    __IO uint32_t LIN_EMC_LEVEL          : 4  ; // 11 : 8
    __IO uint32_t LIN_RX_DELAY           : 4  ; // 15 : 12
    __IO uint32_t LIN_EMC_PD_CUR         : 3  ; // 18 : 16
    __IO uint32_t RESERVED3              : 1  ; // 19 : 19
    __IO uint32_t LIN_EMC_PU_CUR         : 3  ; // 22 : 20
    __IO uint32_t RESERVED2              : 1  ; // 23 : 23
    __IO uint32_t LIN_PDSSOK_EN          : 1  ; // 24 : 24
    __IO uint32_t LIN_HARD_SWITCH        : 1  ; // 25 : 25
    __IO uint32_t RESERVED1              : 2  ; // 27 : 26
    __IO uint32_t LINAA_TESTSEL          : 3  ; // 30 : 28
    __IO uint32_t RESERVED0              : 1  ; // 31 : 31
} TEST_TEST_LIN_CTRL_FIELD_T;

typedef struct
{
    __IO uint32_t LIN_SLOPEDIS           : 1  ; // 0  : 0
    __IO uint32_t LIN_COMPOUT            : 1  ; // 1  : 1
    __IO uint32_t LIN_SLOPE_OK           : 1  ; // 2  : 2
    __IO uint32_t RESERVED0              : 29 ; // 31 : 3
} TEST_TEST_LIN_STATUS_FIELD_T;

typedef struct
{
    __IO uint32_t ADC_VREF_TEST_SEL      : 2  ; // 1  : 0
    __IO uint32_t ADC_VCM_TEST_SEL       : 1  ; // 2  : 2
    __IO uint32_t RESERVED2              : 1  ; // 3  : 3
    __IO uint32_t ADC_TEST_SEL           : 3  ; // 6  : 4
    __IO uint32_t RESERVED1              : 1  ; // 7  : 7
    __IO uint32_t ADC_EXT_CLK_EN         : 1  ; // 8  : 8
    __IO uint32_t RESERVED0              : 23 ; // 31 : 9
} TEST_TEST_ADC_CTRL_FIELD_T;

typedef struct
{
    __IO uint32_t TOP_TP_EN              : 1  ; // 0  : 0
    __IO uint32_t RESERVED1              : 3  ; // 3  : 1
    __IO uint32_t TOP_TP_IP_SEL          : 4  ; // 7  : 4
    __IO uint32_t TOP_TP_HBYTE_SEL       : 1  ; // 8  : 8
    __IO uint32_t TOP_TP_HNIBBLE_SEL     : 1  ; // 9  : 9
    __IO uint32_t TOP_TP_OUT_SEL         : 2  ; // 11 : 10
    __IO uint32_t IP_TP_SEL              : 3  ; // 14 : 12
    __IO uint32_t RESERVED0              : 17 ; // 31 : 15
} TEST_TEST_BUS_CTRL_FIELD_T;

typedef struct
{
    __IO uint32_t LDO15_TRIM             : 3  ; // 2  : 0
    __IO uint32_t RESERVED1              : 1  ; // 3  : 3
    __IO uint32_t LDO15_PORB_SEL         : 2  ; // 5  : 4
    __IO uint32_t RESERVED0              : 26 ; // 31 : 6
} TEST_TEST_LDO15_TRIM_FIELD_T;

typedef struct
{
    __IO uint32_t NVR_DIS_TM             : 1  ; // 0  : 0
    __IO uint32_t NVR_DIS_TM_ALL         : 1  ; // 1  : 1
    __IO uint32_t RESERVED0              : 30 ; // 31 : 2
} TEST_TM_STATUS_FIELD_T;

typedef struct
{
    __IO uint32_t LED_IB_DIAG0_TRIM      : 8  ; // 7  : 0
    __IO uint32_t LED_IB_DIAG1_TRIM      : 8  ; // 15 : 8
    __IO uint32_t LED_IB_DIAG2_TRIM      : 8  ; // 23 : 16
    __IO uint32_t RESERVED0              : 8  ; // 31 : 24
} TEST_TEST_LED_IB_DIAG_TRIM_FIELD_T;

typedef struct
{
    __IO uint32_t BIAS_TRIM              : 3  ; // 2  : 0
    __IO uint32_t RESERVED0              : 29 ; // 31 : 3
} TEST_TEST_BIAS_TRIM_FIELD_T;

typedef struct
{
    __IO uint32_t LED_LDO5_OCP_TRIM      : 2  ; // 1  : 0
    __IO uint32_t RESERVED2              : 2  ; // 3  : 2
    __IO uint32_t LED_LDO5_OUT_TRIM      : 4  ; // 7  : 4
    __IO uint32_t LED_RES_TC_TRIM        : 5  ; // 12 : 8
    __IO uint32_t RESERVED1              : 3  ; // 15 : 13
    __IO uint32_t LED_LDO5_OCP_EN        : 1  ; // 16 : 16
    __IO uint32_t RESERVED0              : 15 ; // 31 : 17
} TEST_TEST_LED_LDO5_TRIM_FIELD_T;

typedef struct
{
    __IO uint32_t RESERVED_T_IDAC_TRIM   : 4  ; // 3  : 0
    __IO uint32_t RESERVED0              : 28 ; // 31 : 4
} TEST_TEST_RESERVED_T_TRIM_FIELD_T;

typedef struct
{
    __IO uint32_t SARADC_TRIM_VAL0       : 6  ; // 5  : 0
    __IO uint32_t RESERVED2              : 2  ; // 7  : 6
    __IO uint32_t SARADC_TRIM_VAL1       : 6  ; // 13 : 8
    __IO uint32_t RESERVED1              : 2  ; // 15 : 14
    __IO uint32_t SARADC_TRIM_VAL2       : 7  ; // 22 : 16
    __IO uint32_t RESERVED0              : 1  ; // 23 : 23
    __IO uint32_t SARADC_TRIM_VAL3       : 8  ; // 31 : 24
} TEST_ADC_TRIM0_FIELD_T;

typedef struct
{
    __IO uint32_t SARADC_TRIM_VAL4       : 8  ; // 7  : 0
    __IO uint32_t RESERVED1              : 8  ; // 15 : 8
    __IO uint32_t SARADC_TRIM_VAL5       : 9  ; // 24 : 16
    __IO uint32_t RESERVED0              : 7  ; // 31 : 25
} TEST_ADC_TRIM1_FIELD_T;

typedef struct
{
    __IO uint32_t SARADC_TRIM_VAL6       : 9  ; // 8  : 0
    __IO uint32_t RESERVED1              : 7  ; // 15 : 9
    __IO uint32_t SARADC_TRIM_VAL7       : 10 ; // 25 : 16
    __IO uint32_t RESERVED0              : 6  ; // 31 : 26
} TEST_ADC_TRIM2_FIELD_T;

typedef struct
{
    __IO uint32_t SARADC_TRIM_VAL8       : 11 ; // 10 : 0
    __IO uint32_t RESERVED1              : 5  ; // 15 : 11
    __IO uint32_t SARADC_TRIM_VAL9       : 12 ; // 27 : 16
    __IO uint32_t RESERVED0              : 4  ; // 31 : 28
} TEST_ADC_TRIM3_FIELD_T;

typedef struct
{
    __IO uint32_t SARADC_TRIM_VAL10      : 12 ; // 11 : 0
    __IO uint32_t RESERVED1              : 4  ; // 15 : 12
    __IO uint32_t SARADC_TRIM_VAL11      : 13 ; // 28 : 16
    __IO uint32_t RESERVED0              : 3  ; // 31 : 29
} TEST_ADC_TRIM4_FIELD_T;

typedef struct
{
    __IO uint32_t SARADC_TRIM_VAL12      : 14 ; // 13 : 0
    __IO uint32_t RESERVED1              : 2  ; // 15 : 14
    __IO uint32_t SARADC_TRIM_VAL13      : 13 ; // 28 : 16
    __IO uint32_t RESERVED0              : 3  ; // 31 : 29
} TEST_ADC_TRIM5_FIELD_T;

typedef struct
{
    __IO uint32_t SARADC_TRIM_VAL14      : 14 ; // 13 : 0
    __IO uint32_t RESERVED1              : 2  ; // 15 : 14
    __IO uint32_t SARADC_TRIM_VAL15      : 15 ; // 30 : 16
    __IO uint32_t RESERVED0              : 1  ; // 31 : 31
} TEST_ADC_TRIM6_FIELD_T;

typedef struct
{
    __IO uint32_t SARADC_TRIM_VAL16      : 16 ; // 15 : 0
    __IO uint32_t RESERVED0              : 16 ; // 31 : 16
} TEST_ADC_TRIM7_FIELD_T;

typedef struct
{
    __IO uint32_t TEST_LOCK              : 1  ; // 0  : 0
    __IO uint32_t RESERVED0              : 31 ; // 31 : 1
} TEST_TEST_LOCK_FIELD_T;

// Register Map Structure Definition
typedef struct
{
    union
    {
        __IO  uint32_t TEST_MODE_CTRL                  ; // 0x0000
        __IO  TEST_TEST_MODE_CTRL_FIELD_T TEST_MODE_CTRL_F;
    };

    union
    {
        __IO  uint32_t TEST_SWD_CTRL                   ; // 0x0004
        __IO  TEST_TEST_SWD_CTRL_FIELD_T TEST_SWD_CTRL_F;
    };

    union
    {
        __IO  uint32_t TEST_PMU_CTRL                   ; // 0x0008
        __IO  TEST_TEST_PMU_CTRL_FIELD_T TEST_PMU_CTRL_F;
    };

    union
    {
        __IO  uint32_t TEST_HRC48M_TRIM                ; // 0x000C
        __IO  TEST_TEST_HRC48M_TRIM_FIELD_T TEST_HRC48M_TRIM_F;
    };

    union
    {
        __IO  uint32_t TEST_LRC32K_TRIM                ; // 0x0010
        __IO  TEST_TEST_LRC32K_TRIM_FIELD_T TEST_LRC32K_TRIM_F;
    };

    union
    {
        __IO  uint32_t TEST_LED_TRIM                   ; // 0x0014
        __IO  TEST_TEST_LED_TRIM_FIELD_T TEST_LED_TRIM_F;
    };

    union
    {
        __IO  uint32_t TEST_ANA_CTRL                   ; // 0x0018
        __IO  TEST_TEST_ANA_CTRL_FIELD_T TEST_ANA_CTRL_F;
    };

    union
    {
        __IO  uint32_t TEST_TPREF_TRIM                 ; // 0x001C
        __IO  TEST_TEST_TPREF_TRIM_FIELD_T TEST_TPREF_TRIM_F;
    };

    union
    {
        __IO  uint32_t TEST_LDO33_TRIM                 ; // 0x0020
        __IO  TEST_TEST_LDO33_TRIM_FIELD_T TEST_LDO33_TRIM_F;
    };

    union
    {
        __IO  uint32_t TEST_OTP_TRIM                   ; // 0x0024
        __IO  TEST_TEST_OTP_TRIM_FIELD_T TEST_OTP_TRIM_F;
    };

    union
    {
        __IO  uint32_t TEST_LIN_TRIM                   ; // 0x0028
        __IO  TEST_TEST_LIN_TRIM_FIELD_T TEST_LIN_TRIM_F;
    };

    union
    {
        __IO  uint32_t TEST_LIN_CTRL                   ; // 0x002C
        __IO  TEST_TEST_LIN_CTRL_FIELD_T TEST_LIN_CTRL_F;
    };

    union
    {
        __IO  uint32_t TEST_LIN_STATUS                 ; // 0x0030
        __IO  TEST_TEST_LIN_STATUS_FIELD_T TEST_LIN_STATUS_F;
    };

    union
    {
        __IO  uint32_t TEST_ADC_CTRL                   ; // 0x0034
        __IO  TEST_TEST_ADC_CTRL_FIELD_T TEST_ADC_CTRL_F;
    };

    union
    {
        __IO  uint32_t TEST_BUS_CTRL                   ; // 0x0038
        __IO  TEST_TEST_BUS_CTRL_FIELD_T TEST_BUS_CTRL_F;
    };

    union
    {
        __IO  uint32_t TEST_LDO15_TRIM                 ; // 0x003C
        __IO  TEST_TEST_LDO15_TRIM_FIELD_T TEST_LDO15_TRIM_F;
    };

    union
    {
        __IO  uint32_t TM_STATUS                       ; // 0x0040
        __IO  TEST_TM_STATUS_FIELD_T TM_STATUS_F       ;
    };

    union
    {
        __IO  uint32_t TEST_LED_IB_DIAG_TRIM           ; // 0x0044
        __IO  TEST_TEST_LED_IB_DIAG_TRIM_FIELD_T TEST_LED_IB_DIAG_TRIM_F;
    };

    union
    {
        __IO  uint32_t TEST_BIAS_TRIM                  ; // 0x0048
        __IO  TEST_TEST_BIAS_TRIM_FIELD_T TEST_BIAS_TRIM_F;
    };

    union
    {
        __IO  uint32_t TEST_LED_LDO5_TRIM              ; // 0x004C
        __IO  TEST_TEST_LED_LDO5_TRIM_FIELD_T TEST_LED_LDO5_TRIM_F;
    };

    union
    {
        __IO  uint32_t TEST_RESERVED_T_TRIM            ; // 0x0050
        __IO  TEST_TEST_RESERVED_T_TRIM_FIELD_T TEST_RESERVED_T_TRIM_F;
    };

    uint32_t RESERVED0[11]                             ;

    union
    {
        __IO  uint32_t ADC_TRIM0                       ; // 0x0080
        __IO  TEST_ADC_TRIM0_FIELD_T ADC_TRIM0_F       ;
    };

    union
    {
        __IO  uint32_t ADC_TRIM1                       ; // 0x0084
        __IO  TEST_ADC_TRIM1_FIELD_T ADC_TRIM1_F       ;
    };

    union
    {
        __IO  uint32_t ADC_TRIM2                       ; // 0x0088
        __IO  TEST_ADC_TRIM2_FIELD_T ADC_TRIM2_F       ;
    };

    union
    {
        __IO  uint32_t ADC_TRIM3                       ; // 0x008C
        __IO  TEST_ADC_TRIM3_FIELD_T ADC_TRIM3_F       ;
    };

    union
    {
        __IO  uint32_t ADC_TRIM4                       ; // 0x0090
        __IO  TEST_ADC_TRIM4_FIELD_T ADC_TRIM4_F       ;
    };

    union
    {
        __IO  uint32_t ADC_TRIM5                       ; // 0x0094
        __IO  TEST_ADC_TRIM5_FIELD_T ADC_TRIM5_F       ;
    };

    union
    {
        __IO  uint32_t ADC_TRIM6                       ; // 0x0098
        __IO  TEST_ADC_TRIM6_FIELD_T ADC_TRIM6_F       ;
    };

    union
    {
        __IO  uint32_t ADC_TRIM7                       ; // 0x009C
        __IO  TEST_ADC_TRIM7_FIELD_T ADC_TRIM7_F       ;
    };

    uint32_t RESERVED1[23]                             ;

    union
    {
        __IO  uint32_t TEST_LOCK                       ; // 0x00FC
        __IO  TEST_TEST_LOCK_FIELD_T TEST_LOCK_F       ;
    };

} TEST_REG_TypeDef;

#endif /* __TEST_DEF_H__ */
