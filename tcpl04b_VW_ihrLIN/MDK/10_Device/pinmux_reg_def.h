/**
 *****************************************************************************
 * @brief   pinmux_reg_def header file.
 *
 * @file    pinmux_reg_def.h
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

#ifndef __PINMUX_DEF_H__
#define __PINMUX_DEF_H__

// Register Offset Macro Definition
#define PINMUX_LED0_CFG_OFFSET                                  0x0000
#define PINMUX_LED1_CFG_OFFSET                                  0x0004
#define PINMUX_LED2_CFG_OFFSET                                  0x0008
#define PINMUX_IO0_CFG_OFFSET                                   0x000C
#define PINMUX_IO1_CFG_OFFSET                                   0x0010
#define PINMUX_IO2_CFG_OFFSET                                   0x0014
#define PINMUX_IO3_CFG_OFFSET                                   0x0018
#define PINMUX_IO4_CFG_OFFSET                                   0x001C
#define PINMUX_IO5_CFG_OFFSET                                   0x0020

// Register Address Macro Definition
#define PINMUX_LED0_CFG_ADDR                                    ( PINMUX_BASE_ADDR + PINMUX_LED0_CFG_OFFSET)
#define PINMUX_LED1_CFG_ADDR                                    ( PINMUX_BASE_ADDR + PINMUX_LED1_CFG_OFFSET)
#define PINMUX_LED2_CFG_ADDR                                    ( PINMUX_BASE_ADDR + PINMUX_LED2_CFG_OFFSET)
#define PINMUX_IO0_CFG_ADDR                                     ( PINMUX_BASE_ADDR + PINMUX_IO0_CFG_OFFSET)
#define PINMUX_IO1_CFG_ADDR                                     ( PINMUX_BASE_ADDR + PINMUX_IO1_CFG_OFFSET)
#define PINMUX_IO2_CFG_ADDR                                     ( PINMUX_BASE_ADDR + PINMUX_IO2_CFG_OFFSET)
#define PINMUX_IO3_CFG_ADDR                                     ( PINMUX_BASE_ADDR + PINMUX_IO3_CFG_OFFSET)
#define PINMUX_IO4_CFG_ADDR                                     ( PINMUX_BASE_ADDR + PINMUX_IO4_CFG_OFFSET)
#define PINMUX_IO5_CFG_ADDR                                     ( PINMUX_BASE_ADDR + PINMUX_IO5_CFG_OFFSET)

// Register Field Macro Definition
#define PINMUX_LED0_CFG_LED0_ASW_SHIFT                          9
#define PINMUX_LED0_CFG_LED0_ASW_MASK                           0x00000200
#define PINMUX_LED0_CFG_LED0_ASW_SET(n)                         (((uint32_t)(n) << 9  ) & 0x00000200)
#define PINMUX_LED0_CFG_LED0_ASW_CLR                            0xFFFFFDFF

#define PINMUX_LED0_CFG_LED0_DR_SHIFT                           8
#define PINMUX_LED0_CFG_LED0_DR_MASK                            0x00000100
#define PINMUX_LED0_CFG_LED0_DR_SET(n)                          (((uint32_t)(n) << 8  ) & 0x00000100)
#define PINMUX_LED0_CFG_LED0_DR_CLR                             0xFFFFFEFF

#define PINMUX_LED0_CFG_LED0_PD_SHIFT                           5
#define PINMUX_LED0_CFG_LED0_PD_MASK                            0x00000020
#define PINMUX_LED0_CFG_LED0_PD_SET(n)                          (((uint32_t)(n) << 5  ) & 0x00000020)
#define PINMUX_LED0_CFG_LED0_PD_CLR                             0xFFFFFFDF

#define PINMUX_LED0_CFG_LED0_PULL_SEL_SHIFT                     4
#define PINMUX_LED0_CFG_LED0_PULL_SEL_MASK                      0x00000010
#define PINMUX_LED0_CFG_LED0_PULL_SEL_SET(n)                    (((uint32_t)(n) << 4  ) & 0x00000010)
#define PINMUX_LED0_CFG_LED0_PULL_SEL_CLR                       0xFFFFFFEF

#define PINMUX_LED0_CFG_LED0_IE_SHIFT                           3
#define PINMUX_LED0_CFG_LED0_IE_MASK                            0x00000008
#define PINMUX_LED0_CFG_LED0_IE_SET(n)                          (((uint32_t)(n) << 3  ) & 0x00000008)
#define PINMUX_LED0_CFG_LED0_IE_CLR                             0xFFFFFFF7

#define PINMUX_LED0_CFG_LED0_SRC_SEL_SHIFT                      0
#define PINMUX_LED0_CFG_LED0_SRC_SEL_MASK                       0x00000007
#define PINMUX_LED0_CFG_LED0_SRC_SEL_SET(n)                     (((uint32_t)(n) << 0  ) & 0x00000007)
#define PINMUX_LED0_CFG_LED0_SRC_SEL_CLR                        0xFFFFFFF8

#define PINMUX_LED1_CFG_LED1_ASW_SHIFT                          9
#define PINMUX_LED1_CFG_LED1_ASW_MASK                           0x00000200
#define PINMUX_LED1_CFG_LED1_ASW_SET(n)                         (((uint32_t)(n) << 9  ) & 0x00000200)
#define PINMUX_LED1_CFG_LED1_ASW_CLR                            0xFFFFFDFF

#define PINMUX_LED1_CFG_LED1_DR_SHIFT                           8
#define PINMUX_LED1_CFG_LED1_DR_MASK                            0x00000100
#define PINMUX_LED1_CFG_LED1_DR_SET(n)                          (((uint32_t)(n) << 8  ) & 0x00000100)
#define PINMUX_LED1_CFG_LED1_DR_CLR                             0xFFFFFEFF

#define PINMUX_LED1_CFG_LED1_PD_SHIFT                           5
#define PINMUX_LED1_CFG_LED1_PD_MASK                            0x00000020
#define PINMUX_LED1_CFG_LED1_PD_SET(n)                          (((uint32_t)(n) << 5  ) & 0x00000020)
#define PINMUX_LED1_CFG_LED1_PD_CLR                             0xFFFFFFDF

#define PINMUX_LED1_CFG_LED1_PULL_SEL_SHIFT                     4
#define PINMUX_LED1_CFG_LED1_PULL_SEL_MASK                      0x00000010
#define PINMUX_LED1_CFG_LED1_PULL_SEL_SET(n)                    (((uint32_t)(n) << 4  ) & 0x00000010)
#define PINMUX_LED1_CFG_LED1_PULL_SEL_CLR                       0xFFFFFFEF

#define PINMUX_LED1_CFG_LED1_IE_SHIFT                           3
#define PINMUX_LED1_CFG_LED1_IE_MASK                            0x00000008
#define PINMUX_LED1_CFG_LED1_IE_SET(n)                          (((uint32_t)(n) << 3  ) & 0x00000008)
#define PINMUX_LED1_CFG_LED1_IE_CLR                             0xFFFFFFF7

#define PINMUX_LED1_CFG_LED1_SRC_SEL_SHIFT                      0
#define PINMUX_LED1_CFG_LED1_SRC_SEL_MASK                       0x00000007
#define PINMUX_LED1_CFG_LED1_SRC_SEL_SET(n)                     (((uint32_t)(n) << 0  ) & 0x00000007)
#define PINMUX_LED1_CFG_LED1_SRC_SEL_CLR                        0xFFFFFFF8

#define PINMUX_LED2_CFG_LED2_ASW_SHIFT                          9
#define PINMUX_LED2_CFG_LED2_ASW_MASK                           0x00000200
#define PINMUX_LED2_CFG_LED2_ASW_SET(n)                         (((uint32_t)(n) << 9  ) & 0x00000200)
#define PINMUX_LED2_CFG_LED2_ASW_CLR                            0xFFFFFDFF

#define PINMUX_LED2_CFG_LED2_DR_SHIFT                           8
#define PINMUX_LED2_CFG_LED2_DR_MASK                            0x00000100
#define PINMUX_LED2_CFG_LED2_DR_SET(n)                          (((uint32_t)(n) << 8  ) & 0x00000100)
#define PINMUX_LED2_CFG_LED2_DR_CLR                             0xFFFFFEFF

#define PINMUX_LED2_CFG_LED2_PD_SHIFT                           5
#define PINMUX_LED2_CFG_LED2_PD_MASK                            0x00000020
#define PINMUX_LED2_CFG_LED2_PD_SET(n)                          (((uint32_t)(n) << 5  ) & 0x00000020)
#define PINMUX_LED2_CFG_LED2_PD_CLR                             0xFFFFFFDF

#define PINMUX_LED2_CFG_LED2_PULL_SEL_SHIFT                     4
#define PINMUX_LED2_CFG_LED2_PULL_SEL_MASK                      0x00000010
#define PINMUX_LED2_CFG_LED2_PULL_SEL_SET(n)                    (((uint32_t)(n) << 4  ) & 0x00000010)
#define PINMUX_LED2_CFG_LED2_PULL_SEL_CLR                       0xFFFFFFEF

#define PINMUX_LED2_CFG_LED2_IE_SHIFT                           3
#define PINMUX_LED2_CFG_LED2_IE_MASK                            0x00000008
#define PINMUX_LED2_CFG_LED2_IE_SET(n)                          (((uint32_t)(n) << 3  ) & 0x00000008)
#define PINMUX_LED2_CFG_LED2_IE_CLR                             0xFFFFFFF7

#define PINMUX_LED2_CFG_LED2_SRC_SEL_SHIFT                      0
#define PINMUX_LED2_CFG_LED2_SRC_SEL_MASK                       0x00000007
#define PINMUX_LED2_CFG_LED2_SRC_SEL_SET(n)                     (((uint32_t)(n) << 0  ) & 0x00000007)
#define PINMUX_LED2_CFG_LED2_SRC_SEL_CLR                        0xFFFFFFF8

#define PINMUX_IO0_CFG_IO0_ASW_SHIFT                            10
#define PINMUX_IO0_CFG_IO0_ASW_MASK                             0x00000400
#define PINMUX_IO0_CFG_IO0_ASW_SET(n)                           (((uint32_t)(n) << 10 ) & 0x00000400)
#define PINMUX_IO0_CFG_IO0_ASW_CLR                              0xFFFFFBFF

#define PINMUX_IO0_CFG_IO0_OD_SHIFT                             9
#define PINMUX_IO0_CFG_IO0_OD_MASK                              0x00000200
#define PINMUX_IO0_CFG_IO0_OD_SET(n)                            (((uint32_t)(n) << 9  ) & 0x00000200)
#define PINMUX_IO0_CFG_IO0_OD_CLR                               0xFFFFFDFF

#define PINMUX_IO0_CFG_IO0_DR_SHIFT                             8
#define PINMUX_IO0_CFG_IO0_DR_MASK                              0x00000100
#define PINMUX_IO0_CFG_IO0_DR_SET(n)                            (((uint32_t)(n) << 8  ) & 0x00000100)
#define PINMUX_IO0_CFG_IO0_DR_CLR                               0xFFFFFEFF

#define PINMUX_IO0_CFG_IO0_PD_SHIFT                             6
#define PINMUX_IO0_CFG_IO0_PD_MASK                              0x00000040
#define PINMUX_IO0_CFG_IO0_PD_SET(n)                            (((uint32_t)(n) << 6  ) & 0x00000040)
#define PINMUX_IO0_CFG_IO0_PD_CLR                               0xFFFFFFBF

#define PINMUX_IO0_CFG_IO0_PU_SHIFT                             5
#define PINMUX_IO0_CFG_IO0_PU_MASK                              0x00000020
#define PINMUX_IO0_CFG_IO0_PU_SET(n)                            (((uint32_t)(n) << 5  ) & 0x00000020)
#define PINMUX_IO0_CFG_IO0_PU_CLR                               0xFFFFFFDF

#define PINMUX_IO0_CFG_IO0_PULL_SEL_SHIFT                       4
#define PINMUX_IO0_CFG_IO0_PULL_SEL_MASK                        0x00000010
#define PINMUX_IO0_CFG_IO0_PULL_SEL_SET(n)                      (((uint32_t)(n) << 4  ) & 0x00000010)
#define PINMUX_IO0_CFG_IO0_PULL_SEL_CLR                         0xFFFFFFEF

#define PINMUX_IO0_CFG_IO0_IE_SHIFT                             3
#define PINMUX_IO0_CFG_IO0_IE_MASK                              0x00000008
#define PINMUX_IO0_CFG_IO0_IE_SET(n)                            (((uint32_t)(n) << 3  ) & 0x00000008)
#define PINMUX_IO0_CFG_IO0_IE_CLR                               0xFFFFFFF7

#define PINMUX_IO0_CFG_IO0_SRC_SEL_SHIFT                        0
#define PINMUX_IO0_CFG_IO0_SRC_SEL_MASK                         0x00000007
#define PINMUX_IO0_CFG_IO0_SRC_SEL_SET(n)                       (((uint32_t)(n) << 0  ) & 0x00000007)
#define PINMUX_IO0_CFG_IO0_SRC_SEL_CLR                          0xFFFFFFF8

#define PINMUX_IO1_CFG_IO1_ASW_SHIFT                            10
#define PINMUX_IO1_CFG_IO1_ASW_MASK                             0x00000400
#define PINMUX_IO1_CFG_IO1_ASW_SET(n)                           (((uint32_t)(n) << 10 ) & 0x00000400)
#define PINMUX_IO1_CFG_IO1_ASW_CLR                              0xFFFFFBFF

#define PINMUX_IO1_CFG_IO1_OD_SHIFT                             9
#define PINMUX_IO1_CFG_IO1_OD_MASK                              0x00000200
#define PINMUX_IO1_CFG_IO1_OD_SET(n)                            (((uint32_t)(n) << 9  ) & 0x00000200)
#define PINMUX_IO1_CFG_IO1_OD_CLR                               0xFFFFFDFF

#define PINMUX_IO1_CFG_IO1_DR_SHIFT                             8
#define PINMUX_IO1_CFG_IO1_DR_MASK                              0x00000100
#define PINMUX_IO1_CFG_IO1_DR_SET(n)                            (((uint32_t)(n) << 8  ) & 0x00000100)
#define PINMUX_IO1_CFG_IO1_DR_CLR                               0xFFFFFEFF

#define PINMUX_IO1_CFG_IO1_PD_SHIFT                             6
#define PINMUX_IO1_CFG_IO1_PD_MASK                              0x00000040
#define PINMUX_IO1_CFG_IO1_PD_SET(n)                            (((uint32_t)(n) << 6  ) & 0x00000040)
#define PINMUX_IO1_CFG_IO1_PD_CLR                               0xFFFFFFBF

#define PINMUX_IO1_CFG_IO1_PU_SHIFT                             5
#define PINMUX_IO1_CFG_IO1_PU_MASK                              0x00000020
#define PINMUX_IO1_CFG_IO1_PU_SET(n)                            (((uint32_t)(n) << 5  ) & 0x00000020)
#define PINMUX_IO1_CFG_IO1_PU_CLR                               0xFFFFFFDF

#define PINMUX_IO1_CFG_IO1_PULL_SEL_SHIFT                       4
#define PINMUX_IO1_CFG_IO1_PULL_SEL_MASK                        0x00000010
#define PINMUX_IO1_CFG_IO1_PULL_SEL_SET(n)                      (((uint32_t)(n) << 4  ) & 0x00000010)
#define PINMUX_IO1_CFG_IO1_PULL_SEL_CLR                         0xFFFFFFEF

#define PINMUX_IO1_CFG_IO1_IE_SHIFT                             3
#define PINMUX_IO1_CFG_IO1_IE_MASK                              0x00000008
#define PINMUX_IO1_CFG_IO1_IE_SET(n)                            (((uint32_t)(n) << 3  ) & 0x00000008)
#define PINMUX_IO1_CFG_IO1_IE_CLR                               0xFFFFFFF7

#define PINMUX_IO1_CFG_IO1_SRC_SEL_SHIFT                        0
#define PINMUX_IO1_CFG_IO1_SRC_SEL_MASK                         0x00000007
#define PINMUX_IO1_CFG_IO1_SRC_SEL_SET(n)                       (((uint32_t)(n) << 0  ) & 0x00000007)
#define PINMUX_IO1_CFG_IO1_SRC_SEL_CLR                          0xFFFFFFF8

#define PINMUX_IO2_CFG_IO2_ASW_SHIFT                            10
#define PINMUX_IO2_CFG_IO2_ASW_MASK                             0x00000400
#define PINMUX_IO2_CFG_IO2_ASW_SET(n)                           (((uint32_t)(n) << 10 ) & 0x00000400)
#define PINMUX_IO2_CFG_IO2_ASW_CLR                              0xFFFFFBFF

#define PINMUX_IO2_CFG_IO2_OD_SHIFT                             9
#define PINMUX_IO2_CFG_IO2_OD_MASK                              0x00000200
#define PINMUX_IO2_CFG_IO2_OD_SET(n)                            (((uint32_t)(n) << 9  ) & 0x00000200)
#define PINMUX_IO2_CFG_IO2_OD_CLR                               0xFFFFFDFF

#define PINMUX_IO2_CFG_IO2_PD_SHIFT                             6
#define PINMUX_IO2_CFG_IO2_PD_MASK                              0x00000040
#define PINMUX_IO2_CFG_IO2_PD_SET(n)                            (((uint32_t)(n) << 6  ) & 0x00000040)
#define PINMUX_IO2_CFG_IO2_PD_CLR                               0xFFFFFFBF

#define PINMUX_IO2_CFG_IO2_PU_SHIFT                             5
#define PINMUX_IO2_CFG_IO2_PU_MASK                              0x00000020
#define PINMUX_IO2_CFG_IO2_PU_SET(n)                            (((uint32_t)(n) << 5  ) & 0x00000020)
#define PINMUX_IO2_CFG_IO2_PU_CLR                               0xFFFFFFDF

#define PINMUX_IO2_CFG_IO2_PULL_SEL_SHIFT                       4
#define PINMUX_IO2_CFG_IO2_PULL_SEL_MASK                        0x00000010
#define PINMUX_IO2_CFG_IO2_PULL_SEL_SET(n)                      (((uint32_t)(n) << 4  ) & 0x00000010)
#define PINMUX_IO2_CFG_IO2_PULL_SEL_CLR                         0xFFFFFFEF

#define PINMUX_IO2_CFG_IO2_IE_SHIFT                             3
#define PINMUX_IO2_CFG_IO2_IE_MASK                              0x00000008
#define PINMUX_IO2_CFG_IO2_IE_SET(n)                            (((uint32_t)(n) << 3  ) & 0x00000008)
#define PINMUX_IO2_CFG_IO2_IE_CLR                               0xFFFFFFF7

#define PINMUX_IO2_CFG_IO2_SRC_SEL_SHIFT                        0
#define PINMUX_IO2_CFG_IO2_SRC_SEL_MASK                         0x00000007
#define PINMUX_IO2_CFG_IO2_SRC_SEL_SET(n)                       (((uint32_t)(n) << 0  ) & 0x00000007)
#define PINMUX_IO2_CFG_IO2_SRC_SEL_CLR                          0xFFFFFFF8

#define PINMUX_IO3_CFG_IO3_ASW_SHIFT                            10
#define PINMUX_IO3_CFG_IO3_ASW_MASK                             0x00000400
#define PINMUX_IO3_CFG_IO3_ASW_SET(n)                           (((uint32_t)(n) << 10 ) & 0x00000400)
#define PINMUX_IO3_CFG_IO3_ASW_CLR                              0xFFFFFBFF

#define PINMUX_IO3_CFG_IO3_OD_SHIFT                             9
#define PINMUX_IO3_CFG_IO3_OD_MASK                              0x00000200
#define PINMUX_IO3_CFG_IO3_OD_SET(n)                            (((uint32_t)(n) << 9  ) & 0x00000200)
#define PINMUX_IO3_CFG_IO3_OD_CLR                               0xFFFFFDFF

#define PINMUX_IO3_CFG_IO3_PD_SHIFT                             6
#define PINMUX_IO3_CFG_IO3_PD_MASK                              0x00000040
#define PINMUX_IO3_CFG_IO3_PD_SET(n)                            (((uint32_t)(n) << 6  ) & 0x00000040)
#define PINMUX_IO3_CFG_IO3_PD_CLR                               0xFFFFFFBF

#define PINMUX_IO3_CFG_IO3_PU_SHIFT                             5
#define PINMUX_IO3_CFG_IO3_PU_MASK                              0x00000020
#define PINMUX_IO3_CFG_IO3_PU_SET(n)                            (((uint32_t)(n) << 5  ) & 0x00000020)
#define PINMUX_IO3_CFG_IO3_PU_CLR                               0xFFFFFFDF

#define PINMUX_IO3_CFG_IO3_PULL_SEL_SHIFT                       4
#define PINMUX_IO3_CFG_IO3_PULL_SEL_MASK                        0x00000010
#define PINMUX_IO3_CFG_IO3_PULL_SEL_SET(n)                      (((uint32_t)(n) << 4  ) & 0x00000010)
#define PINMUX_IO3_CFG_IO3_PULL_SEL_CLR                         0xFFFFFFEF

#define PINMUX_IO3_CFG_IO3_IE_SHIFT                             3
#define PINMUX_IO3_CFG_IO3_IE_MASK                              0x00000008
#define PINMUX_IO3_CFG_IO3_IE_SET(n)                            (((uint32_t)(n) << 3  ) & 0x00000008)
#define PINMUX_IO3_CFG_IO3_IE_CLR                               0xFFFFFFF7

#define PINMUX_IO3_CFG_IO3_SRC_SEL_SHIFT                        0
#define PINMUX_IO3_CFG_IO3_SRC_SEL_MASK                         0x00000007
#define PINMUX_IO3_CFG_IO3_SRC_SEL_SET(n)                       (((uint32_t)(n) << 0  ) & 0x00000007)
#define PINMUX_IO3_CFG_IO3_SRC_SEL_CLR                          0xFFFFFFF8

#define PINMUX_IO4_CFG_IO4_ASW_SHIFT                            10
#define PINMUX_IO4_CFG_IO4_ASW_MASK                             0x00000400
#define PINMUX_IO4_CFG_IO4_ASW_SET(n)                           (((uint32_t)(n) << 10 ) & 0x00000400)
#define PINMUX_IO4_CFG_IO4_ASW_CLR                              0xFFFFFBFF

#define PINMUX_IO4_CFG_IO4_OD_SHIFT                             9
#define PINMUX_IO4_CFG_IO4_OD_MASK                              0x00000200
#define PINMUX_IO4_CFG_IO4_OD_SET(n)                            (((uint32_t)(n) << 9  ) & 0x00000200)
#define PINMUX_IO4_CFG_IO4_OD_CLR                               0xFFFFFDFF

#define PINMUX_IO4_CFG_IO4_PD_SHIFT                             6
#define PINMUX_IO4_CFG_IO4_PD_MASK                              0x00000040
#define PINMUX_IO4_CFG_IO4_PD_SET(n)                            (((uint32_t)(n) << 6  ) & 0x00000040)
#define PINMUX_IO4_CFG_IO4_PD_CLR                               0xFFFFFFBF

#define PINMUX_IO4_CFG_IO4_PU_SHIFT                             5
#define PINMUX_IO4_CFG_IO4_PU_MASK                              0x00000020
#define PINMUX_IO4_CFG_IO4_PU_SET(n)                            (((uint32_t)(n) << 5  ) & 0x00000020)
#define PINMUX_IO4_CFG_IO4_PU_CLR                               0xFFFFFFDF

#define PINMUX_IO4_CFG_IO4_PULL_SEL_SHIFT                       4
#define PINMUX_IO4_CFG_IO4_PULL_SEL_MASK                        0x00000010
#define PINMUX_IO4_CFG_IO4_PULL_SEL_SET(n)                      (((uint32_t)(n) << 4  ) & 0x00000010)
#define PINMUX_IO4_CFG_IO4_PULL_SEL_CLR                         0xFFFFFFEF

#define PINMUX_IO4_CFG_IO4_IE_SHIFT                             3
#define PINMUX_IO4_CFG_IO4_IE_MASK                              0x00000008
#define PINMUX_IO4_CFG_IO4_IE_SET(n)                            (((uint32_t)(n) << 3  ) & 0x00000008)
#define PINMUX_IO4_CFG_IO4_IE_CLR                               0xFFFFFFF7

#define PINMUX_IO4_CFG_IO4_SRC_SEL_SHIFT                        0
#define PINMUX_IO4_CFG_IO4_SRC_SEL_MASK                         0x00000007
#define PINMUX_IO4_CFG_IO4_SRC_SEL_SET(n)                       (((uint32_t)(n) << 0  ) & 0x00000007)
#define PINMUX_IO4_CFG_IO4_SRC_SEL_CLR                          0xFFFFFFF8

#define PINMUX_IO5_CFG_IO5_ASW_SHIFT                            10
#define PINMUX_IO5_CFG_IO5_ASW_MASK                             0x00000400
#define PINMUX_IO5_CFG_IO5_ASW_SET(n)                           (((uint32_t)(n) << 10 ) & 0x00000400)
#define PINMUX_IO5_CFG_IO5_ASW_CLR                              0xFFFFFBFF

#define PINMUX_IO5_CFG_IO5_OD_SHIFT                             9
#define PINMUX_IO5_CFG_IO5_OD_MASK                              0x00000200
#define PINMUX_IO5_CFG_IO5_OD_SET(n)                            (((uint32_t)(n) << 9  ) & 0x00000200)
#define PINMUX_IO5_CFG_IO5_OD_CLR                               0xFFFFFDFF

#define PINMUX_IO5_CFG_IO5_PD_SHIFT                             6
#define PINMUX_IO5_CFG_IO5_PD_MASK                              0x00000040
#define PINMUX_IO5_CFG_IO5_PD_SET(n)                            (((uint32_t)(n) << 6  ) & 0x00000040)
#define PINMUX_IO5_CFG_IO5_PD_CLR                               0xFFFFFFBF

#define PINMUX_IO5_CFG_IO5_PU_SHIFT                             5
#define PINMUX_IO5_CFG_IO5_PU_MASK                              0x00000020
#define PINMUX_IO5_CFG_IO5_PU_SET(n)                            (((uint32_t)(n) << 5  ) & 0x00000020)
#define PINMUX_IO5_CFG_IO5_PU_CLR                               0xFFFFFFDF

#define PINMUX_IO5_CFG_IO5_PULL_SEL_SHIFT                       4
#define PINMUX_IO5_CFG_IO5_PULL_SEL_MASK                        0x00000010
#define PINMUX_IO5_CFG_IO5_PULL_SEL_SET(n)                      (((uint32_t)(n) << 4  ) & 0x00000010)
#define PINMUX_IO5_CFG_IO5_PULL_SEL_CLR                         0xFFFFFFEF

#define PINMUX_IO5_CFG_IO5_IE_SHIFT                             3
#define PINMUX_IO5_CFG_IO5_IE_MASK                              0x00000008
#define PINMUX_IO5_CFG_IO5_IE_SET(n)                            (((uint32_t)(n) << 3  ) & 0x00000008)
#define PINMUX_IO5_CFG_IO5_IE_CLR                               0xFFFFFFF7

#define PINMUX_IO5_CFG_IO5_SRC_SEL_SHIFT                        0
#define PINMUX_IO5_CFG_IO5_SRC_SEL_MASK                         0x00000007
#define PINMUX_IO5_CFG_IO5_SRC_SEL_SET(n)                       (((uint32_t)(n) << 0  ) & 0x00000007)
#define PINMUX_IO5_CFG_IO5_SRC_SEL_CLR                          0xFFFFFFF8

// Register Structure Definition
typedef struct
{
    __IO uint32_t LED0_SRC_SEL    : 3  ; // 2  : 0
    __IO uint32_t LED0_IE         : 1  ; // 3  : 3
    __IO uint32_t LED0_PULL_SEL   : 1  ; // 4  : 4
    __IO uint32_t LED0_PD         : 1  ; // 5  : 5
    __IO uint32_t RESERVED1       : 2  ; // 7  : 6
    __IO uint32_t LED0_DR         : 1  ; // 8  : 8
    __IO uint32_t LED0_ASW        : 1  ; // 9  : 9
    __IO uint32_t RESERVED0       : 22 ; // 31 : 10
} PINMUX_LED0_CFG_FIELD_T;

typedef struct
{
    __IO uint32_t LED1_SRC_SEL    : 3  ; // 2  : 0
    __IO uint32_t LED1_IE         : 1  ; // 3  : 3
    __IO uint32_t LED1_PULL_SEL   : 1  ; // 4  : 4
    __IO uint32_t LED1_PD         : 1  ; // 5  : 5
    __IO uint32_t RESERVED1       : 2  ; // 7  : 6
    __IO uint32_t LED1_DR         : 1  ; // 8  : 8
    __IO uint32_t LED1_ASW        : 1  ; // 9  : 9
    __IO uint32_t RESERVED0       : 22 ; // 31 : 10
} PINMUX_LED1_CFG_FIELD_T;

typedef struct
{
    __IO uint32_t LED2_SRC_SEL    : 3  ; // 2  : 0
    __IO uint32_t LED2_IE         : 1  ; // 3  : 3
    __IO uint32_t LED2_PULL_SEL   : 1  ; // 4  : 4
    __IO uint32_t LED2_PD         : 1  ; // 5  : 5
    __IO uint32_t RESERVED1       : 2  ; // 7  : 6
    __IO uint32_t LED2_DR         : 1  ; // 8  : 8
    __IO uint32_t LED2_ASW        : 1  ; // 9  : 9
    __IO uint32_t RESERVED0       : 22 ; // 31 : 10
} PINMUX_LED2_CFG_FIELD_T;

typedef struct
{
    __IO uint32_t IO0_SRC_SEL     : 3  ; // 2  : 0
    __IO uint32_t IO0_IE          : 1  ; // 3  : 3
    __IO uint32_t IO0_PULL_SEL    : 1  ; // 4  : 4
    __IO uint32_t IO0_PU          : 1  ; // 5  : 5
    __IO uint32_t IO0_PD          : 1  ; // 6  : 6
    __IO uint32_t RESERVED1       : 1  ; // 7  : 7
    __IO uint32_t IO0_DR          : 1  ; // 8  : 8
    __IO uint32_t IO0_OD          : 1  ; // 9  : 9
    __IO uint32_t IO0_ASW         : 1  ; // 10 : 10
    __IO uint32_t RESERVED0       : 21 ; // 31 : 11
} PINMUX_IO0_CFG_FIELD_T;

typedef struct
{
    __IO uint32_t IO1_SRC_SEL     : 3  ; // 2  : 0
    __IO uint32_t IO1_IE          : 1  ; // 3  : 3
    __IO uint32_t IO1_PULL_SEL    : 1  ; // 4  : 4
    __IO uint32_t IO1_PU          : 1  ; // 5  : 5
    __IO uint32_t IO1_PD          : 1  ; // 6  : 6
    __IO uint32_t RESERVED1       : 1  ; // 7  : 7
    __IO uint32_t IO1_DR          : 1  ; // 8  : 8
    __IO uint32_t IO1_OD          : 1  ; // 9  : 9
    __IO uint32_t IO1_ASW         : 1  ; // 10 : 10
    __IO uint32_t RESERVED0       : 21 ; // 31 : 11
} PINMUX_IO1_CFG_FIELD_T;

typedef struct
{
    __IO uint32_t IO2_SRC_SEL     : 3  ; // 2  : 0
    __IO uint32_t IO2_IE          : 1  ; // 3  : 3
    __IO uint32_t IO2_PULL_SEL    : 1  ; // 4  : 4
    __IO uint32_t IO2_PU          : 1  ; // 5  : 5
    __IO uint32_t IO2_PD          : 1  ; // 6  : 6
    __IO uint32_t RESERVED1       : 2  ; // 8  : 7
    __IO uint32_t IO2_OD          : 1  ; // 9  : 9
    __IO uint32_t IO2_ASW         : 1  ; // 10 : 10
    __IO uint32_t RESERVED0       : 21 ; // 31 : 11
} PINMUX_IO2_CFG_FIELD_T;

typedef struct
{
    __IO uint32_t IO3_SRC_SEL     : 3  ; // 2  : 0
    __IO uint32_t IO3_IE          : 1  ; // 3  : 3
    __IO uint32_t IO3_PULL_SEL    : 1  ; // 4  : 4
    __IO uint32_t IO3_PU          : 1  ; // 5  : 5
    __IO uint32_t IO3_PD          : 1  ; // 6  : 6
    __IO uint32_t RESERVED1       : 2  ; // 8  : 7
    __IO uint32_t IO3_OD          : 1  ; // 9  : 9
    __IO uint32_t IO3_ASW         : 1  ; // 10 : 10
    __IO uint32_t RESERVED0       : 21 ; // 31 : 11
} PINMUX_IO3_CFG_FIELD_T;

typedef struct
{
    __IO uint32_t IO4_SRC_SEL     : 3  ; // 2  : 0
    __IO uint32_t IO4_IE          : 1  ; // 3  : 3
    __IO uint32_t IO4_PULL_SEL    : 1  ; // 4  : 4
    __IO uint32_t IO4_PU          : 1  ; // 5  : 5
    __IO uint32_t IO4_PD          : 1  ; // 6  : 6
    __IO uint32_t RESERVED1       : 2  ; // 8  : 7
    __IO uint32_t IO4_OD          : 1  ; // 9  : 9
    __IO uint32_t IO4_ASW         : 1  ; // 10 : 10
    __IO uint32_t RESERVED0       : 21 ; // 31 : 11
} PINMUX_IO4_CFG_FIELD_T;

typedef struct
{
    __IO uint32_t IO5_SRC_SEL     : 3  ; // 2  : 0
    __IO uint32_t IO5_IE          : 1  ; // 3  : 3
    __IO uint32_t IO5_PULL_SEL    : 1  ; // 4  : 4
    __IO uint32_t IO5_PU          : 1  ; // 5  : 5
    __IO uint32_t IO5_PD          : 1  ; // 6  : 6
    __IO uint32_t RESERVED1       : 2  ; // 8  : 7
    __IO uint32_t IO5_OD          : 1  ; // 9  : 9
    __IO uint32_t IO5_ASW         : 1  ; // 10 : 10
    __IO uint32_t RESERVED0       : 21 ; // 31 : 11
} PINMUX_IO5_CFG_FIELD_T;

// Register Map Structure Definition
typedef struct
{
    union
    {
        __IO  uint32_t LED0_CFG                        ; // 0x0000
        __IO  PINMUX_LED0_CFG_FIELD_T LED0_CFG_F       ;
    };

    union
    {
        __IO  uint32_t LED1_CFG                        ; // 0x0004
        __IO  PINMUX_LED1_CFG_FIELD_T LED1_CFG_F       ;
    };

    union
    {
        __IO  uint32_t LED2_CFG                        ; // 0x0008
        __IO  PINMUX_LED2_CFG_FIELD_T LED2_CFG_F       ;
    };

    union
    {
        __IO  uint32_t IO0_CFG                         ; // 0x000C
        __IO  PINMUX_IO0_CFG_FIELD_T IO0_CFG_F         ;
    };

    union
    {
        __IO  uint32_t IO1_CFG                         ; // 0x0010
        __IO  PINMUX_IO1_CFG_FIELD_T IO1_CFG_F         ;
    };

    union
    {
        __IO  uint32_t IO2_CFG                         ; // 0x0014
        __IO  PINMUX_IO2_CFG_FIELD_T IO2_CFG_F         ;
    };

    union
    {
        __IO  uint32_t IO3_CFG                         ; // 0x0018
        __IO  PINMUX_IO3_CFG_FIELD_T IO3_CFG_F         ;
    };

    union
    {
        __IO  uint32_t IO4_CFG                         ; // 0x001C
        __IO  PINMUX_IO4_CFG_FIELD_T IO4_CFG_F         ;
    };

    union
    {
        __IO  uint32_t IO5_CFG                         ; // 0x0020
        __IO  PINMUX_IO5_CFG_FIELD_T IO5_CFG_F         ;
    };

} PINMUX_REG_TypeDef;

#endif /* __PINMUX_DEF_H__ */
