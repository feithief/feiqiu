/**
 *****************************************************************************
 * @brief   tim_lite_reg_def header file.
 *
 * @file    tim_lite_reg_def.h
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

#ifndef __TIM_LITE_DEF_H__
#define __TIM_LITE_DEF_H__

// Register Offset Macro Definition
#define TIM_LITE_CTRL_OFFSET                                    0x0000
#define TIM_LITE_PRESCALE_OFFSET                                0x0004
#define TIM_LITE_REPEAT_OFFSET                                  0x0008
#define TIM_LITE_INIT_VAL_OFFSET                                0x000C
#define TIM_LITE_CNT_VAL_OFFSET                                 0x0010
#define TIM_LITE_IMR_OFFSET                                     0x0014
#define TIM_LITE_ICR_OFFSET                                     0x0018
#define TIM_LITE_ISR_OFFSET                                     0x001C
#define TIM_LITE_IRSR_OFFSET                                    0x0020
#define TIM_LITE_RAW_CNT_VAL_OFFSET                             0x0024

// Register Address Macro Definition
#define TIM_LITE_CTRL_ADDR                                      ( TIM_LITE_BASE_ADDR + TIM_LITE_CTRL_OFFSET)
#define TIM_LITE_PRESCALE_ADDR                                  ( TIM_LITE_BASE_ADDR + TIM_LITE_PRESCALE_OFFSET)
#define TIM_LITE_REPEAT_ADDR                                    ( TIM_LITE_BASE_ADDR + TIM_LITE_REPEAT_OFFSET)
#define TIM_LITE_INIT_VAL_ADDR                                  ( TIM_LITE_BASE_ADDR + TIM_LITE_INIT_VAL_OFFSET)
#define TIM_LITE_CNT_VAL_ADDR                                   ( TIM_LITE_BASE_ADDR + TIM_LITE_CNT_VAL_OFFSET)
#define TIM_LITE_IMR_ADDR                                       ( TIM_LITE_BASE_ADDR + TIM_LITE_IMR_OFFSET)
#define TIM_LITE_ICR_ADDR                                       ( TIM_LITE_BASE_ADDR + TIM_LITE_ICR_OFFSET)
#define TIM_LITE_ISR_ADDR                                       ( TIM_LITE_BASE_ADDR + TIM_LITE_ISR_OFFSET)
#define TIM_LITE_IRSR_ADDR                                      ( TIM_LITE_BASE_ADDR + TIM_LITE_IRSR_OFFSET)
#define TIM_LITE_RAW_CNT_VAL_ADDR                               ( TIM_LITE_BASE_ADDR + TIM_LITE_RAW_CNT_VAL_OFFSET)

// Register Field Macro Definition
#define TIM_LITE_CTRL_TRIG_EN_SHIFT                                         7
#define TIM_LITE_CTRL_TRIG_EN_MASK                                          0x00000080
#define TIM_LITE_CTRL_TRIG_EN_SET(n)                                        (((uint32_t)(n) << 7  ) & 0x00000080)
#define TIM_LITE_CTRL_TRIG_EN_CLR                                           0xFFFFFF7F

#define TIM_LITE_CTRL_PAUSE_SHIFT                                           6
#define TIM_LITE_CTRL_PAUSE_MASK                                            0x00000040
#define TIM_LITE_CTRL_PAUSE_SET(n)                                          (((uint32_t)(n) << 6  ) & 0x00000040)
#define TIM_LITE_CTRL_PAUSE_CLR                                             0xFFFFFFBF

#define TIM_LITE_CTRL_STP_SHIFT                                             5
#define TIM_LITE_CTRL_STP_MASK                                              0x00000020
#define TIM_LITE_CTRL_STP_SET(n)                                            (((uint32_t)(n) << 5  ) & 0x00000020)
#define TIM_LITE_CTRL_STP_CLR                                               0xFFFFFFDF

#define TIM_LITE_CTRL_CNT_TRIG_SHIFT                                        3
#define TIM_LITE_CTRL_CNT_TRIG_MASK                                         0x00000008
#define TIM_LITE_CTRL_CNT_TRIG_SET(n)                                       (((uint32_t)(n) << 3  ) & 0x00000008)
#define TIM_LITE_CTRL_CNT_TRIG_CLR                                          0xFFFFFFF7

#define TIM_LITE_CTRL_INIT_VALUE_LD_SHIFT                                   2
#define TIM_LITE_CTRL_INIT_VALUE_LD_MASK                                    0x00000004
#define TIM_LITE_CTRL_INIT_VALUE_LD_SET(n)                                  (((uint32_t)(n) << 2  ) & 0x00000004)
#define TIM_LITE_CTRL_INIT_VALUE_LD_CLR                                     0xFFFFFFFB

#define TIM_LITE_CTRL_LOOP_DIS_SHIFT                                        1
#define TIM_LITE_CTRL_LOOP_DIS_MASK                                         0x00000002
#define TIM_LITE_CTRL_LOOP_DIS_SET(n)                                       (((uint32_t)(n) << 1  ) & 0x00000002)
#define TIM_LITE_CTRL_LOOP_DIS_CLR                                          0xFFFFFFFD

#define TIM_LITE_CTRL_EN_SHIFT                                              0
#define TIM_LITE_CTRL_EN_MASK                                               0x00000001
#define TIM_LITE_CTRL_EN_SET(n)                                             (((uint32_t)(n) << 0  ) & 0x00000001)
#define TIM_LITE_CTRL_EN_CLR                                                0xFFFFFFFE

#define TIM_LITE_INIT_VAL_CNT_INIT_VAL_SHIFT                                0
#define TIM_LITE_INIT_VAL_CNT_INIT_VAL_MASK                                 0x0000FFFF
#define TIM_LITE_INIT_VAL_CNT_INIT_VAL_SET(n)                               (((uint32_t)(n) << 0  ) & 0x0000FFFF)
#define TIM_LITE_INIT_VAL_CNT_INIT_VAL_CLR                                  0xFFFF0000

#define TIM_LITE_CNT_VAL_CNT_VAL_SHIFT                                      0
#define TIM_LITE_CNT_VAL_CNT_VAL_MASK                                       0x0000FFFF
#define TIM_LITE_CNT_VAL_CNT_VAL_SET(n)                                     (((uint32_t)(n) << 0  ) & 0x0000FFFF)
#define TIM_LITE_CNT_VAL_CNT_VAL_CLR                                        0xFFFF0000

#define TIM_LITE_IMR_CNT_UDF_INT_MSK_SHIFT                                  0
#define TIM_LITE_IMR_CNT_UDF_INT_MSK_MASK                                   0x00000001
#define TIM_LITE_IMR_CNT_UDF_INT_MSK_SET(n)                                 (((uint32_t)(n) << 0  ) & 0x00000001)
#define TIM_LITE_IMR_CNT_UDF_INT_MSK_CLR                                    0xFFFFFFFE

#define TIM_LITE_ICR_CNT_UDF_INT_CLR_SHIFT                                  0
#define TIM_LITE_ICR_CNT_UDF_INT_CLR_MASK                                   0x00000001
#define TIM_LITE_ICR_CNT_UDF_INT_CLR_SET(n)                                 (((uint32_t)(n) << 0  ) & 0x00000001)
#define TIM_LITE_ICR_CNT_UDF_INT_CLR_CLR                                    0xFFFFFFFE

#define TIM_LITE_ISR_CNT_UDF_INT_STATUS_SHIFT                               0
#define TIM_LITE_ISR_CNT_UDF_INT_STATUS_MASK                                0x00000001
#define TIM_LITE_ISR_CNT_UDF_INT_STATUS_SET(n)                              (((uint32_t)(n) << 0  ) & 0x00000001)
#define TIM_LITE_ISR_CNT_UDF_INT_STATUS_CLR                                 0xFFFFFFFE

#define TIM_LITE_IRSR_CNT_UDF_INT_RAW_STATUS_SHIFT                          0
#define TIM_LITE_IRSR_CNT_UDF_INT_RAW_STATUS_MASK                           0x00000001
#define TIM_LITE_IRSR_CNT_UDF_INT_RAW_STATUS_SET(n)                         (((uint32_t)(n) << 0  ) & 0x00000001)
#define TIM_LITE_IRSR_CNT_UDF_INT_RAW_STATUS_CLR                            0xFFFFFFFE

#define TIM_LITE_RAW_CNT_VAL_RAW_CNT_VAL_SHIFT                              0
#define TIM_LITE_RAW_CNT_VAL_RAW_CNT_VAL_MASK                               0x0000FFFF
#define TIM_LITE_RAW_CNT_VAL_RAW_CNT_VAL_SET(n)                             (((uint32_t)(n) << 0  ) & 0x0000FFFF)
#define TIM_LITE_RAW_CNT_VAL_RAW_CNT_VAL_CLR                                0xFFFF0000

// Register Structure Definition
typedef struct
{
    __IO uint32_t EN                     : 1  ; // 0  : 0
    __IO uint32_t LOOP_DIS               : 1  ; // 1  : 1
    __IO uint32_t INIT_VALUE_LD          : 1  ; // 2  : 2
    __IO uint32_t CNT_TRIG               : 1  ; // 3  : 3
    __IO uint32_t RESERVED1              : 1  ; // 4  : 4
    __IO uint32_t STP                    : 1  ; // 5  : 5
    __IO uint32_t PAUSE                  : 1  ; // 6  : 6
    __IO uint32_t TRIG_EN                : 1  ; // 7  : 7
    __IO uint32_t RESERVED0              : 24 ; // 31 : 8
} TIM_LITE_CTRL_FIELD_T;

typedef struct
{
    __IO uint32_t RESERVED0              : 32 ; // 31 : 0
} TIM_LITE_PRESCALE_FIELD_T;

typedef struct
{
    __IO uint32_t RESERVED0              : 32 ; // 31 : 0
} TIM_LITE_REPEAT_FIELD_T;

typedef struct
{
    __IO uint32_t CNT_INIT_VAL           : 16 ; // 15 : 0
    __IO uint32_t RESERVED0              : 16 ; // 31 : 16
} TIM_LITE_INIT_VAL_FIELD_T;

typedef struct
{
    __IO uint32_t CNT_VAL                : 16 ; // 15 : 0
    __IO uint32_t RESERVED0              : 16 ; // 31 : 16
} TIM_LITE_CNT_VAL_FIELD_T;

typedef struct
{
    __IO uint32_t CNT_UDF_INT_MSK        : 1  ; // 0  : 0
    __IO uint32_t RESERVED0              : 31 ; // 31 : 1
} TIM_LITE_IMR_FIELD_T;

typedef struct
{
    __IO uint32_t CNT_UDF_INT_CLR        : 1  ; // 0  : 0
    __IO uint32_t RESERVED0              : 31 ; // 31 : 1
} TIM_LITE_ICR_FIELD_T;

typedef struct
{
    __IO uint32_t CNT_UDF_INT_STATUS     : 1  ; // 0  : 0
    __IO uint32_t RESERVED0              : 31 ; // 31 : 1
} TIM_LITE_ISR_FIELD_T;

typedef struct
{
    __IO uint32_t CNT_UDF_INT_RAW_STATUS : 1  ; // 0  : 0
    __IO uint32_t RESERVED0              : 31 ; // 31 : 1
} TIM_LITE_IRSR_FIELD_T;

typedef struct
{
    __IO uint32_t RAW_CNT_VAL            : 16 ; // 15 : 0
    __IO uint32_t RESERVED0              : 16 ; // 31 : 16
} TIM_LITE_RAW_CNT_VAL_FIELD_T;

// Register Map Structure Definition
typedef struct
{
    union
    {
        __IO  uint32_t CTRL                            ; // 0x0000
        __IO  TIM_LITE_CTRL_FIELD_T CTRL_F             ;
    };

    union
    {
        __IO  uint32_t PRESCALE                        ; // 0x0004
        __IO  TIM_LITE_PRESCALE_FIELD_T PRESCALE_F     ;
    };

    union
    {
        __IO  uint32_t REPEAT                          ; // 0x0008
        __IO  TIM_LITE_REPEAT_FIELD_T REPEAT_F         ;
    };

    union
    {
        __IO  uint32_t INIT_VAL                        ; // 0x000C
        __IO  TIM_LITE_INIT_VAL_FIELD_T INIT_VAL_F     ;
    };

    union
    {
        __IO  uint32_t CNT_VAL                         ; // 0x0010
        __IO  TIM_LITE_CNT_VAL_FIELD_T CNT_VAL_F       ;
    };

    union
    {
        __IO  uint32_t IMR                             ; // 0x0014
        __IO  TIM_LITE_IMR_FIELD_T IMR_F               ;
    };

    union
    {
        __IO  uint32_t ICR                             ; // 0x0018
        __IO  TIM_LITE_ICR_FIELD_T ICR_F               ;
    };

    union
    {
        __IO  uint32_t ISR                             ; // 0x001C
        __IO  TIM_LITE_ISR_FIELD_T ISR_F               ;
    };

    union
    {
        __IO  uint32_t IRSR                            ; // 0x0020
        __IO  TIM_LITE_IRSR_FIELD_T IRSR_F             ;
    };

    union
    {
        __IO  uint32_t RAW_CNT_VAL                     ; // 0x0024
        __IO  TIM_LITE_RAW_CNT_VAL_FIELD_T RAW_CNT_VAL_F;
    };

} TIM_LITE_REG_TypeDef;

#endif /* __TIM_LITE_DEF_H__ */
