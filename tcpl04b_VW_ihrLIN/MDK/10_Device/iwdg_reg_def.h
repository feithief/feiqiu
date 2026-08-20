/**
 *****************************************************************************
 * @brief   iwdg_reg_def header file.
 *
 * @file    iwdg_reg_def.h
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

#ifndef __IWDG_DEF_H__
#define __IWDG_DEF_H__

// Register Offset Macro Definition
#define IWDG_CTRL_OFFSET                                        0x0000
#define IWDG_CNT_MAX_OFFSET                                     0x0004
#define IWDG_WIN_MAX_OFFSET                                     0x0008
#define IWDG_WIN_MIN_OFFSET                                     0x000C
#define IWDG_CCR_OFFSET                                         0x0010
#define IWDG_IRQ_OFFSET                                         0x0014
#define IWDG_LOCK_OFFSET                                        0x0018
#define IWDG_STATUS_OFFSET                                      0x001C

// Register Address Macro Definition
#define IWDG_CTRL_ADDR                                          ( IWDG_BASE_ADDR + IWDG_CTRL_OFFSET)
#define IWDG_CNT_MAX_ADDR                                       ( IWDG_BASE_ADDR + IWDG_CNT_MAX_OFFSET)
#define IWDG_WIN_MAX_ADDR                                       ( IWDG_BASE_ADDR + IWDG_WIN_MAX_OFFSET)
#define IWDG_WIN_MIN_ADDR                                       ( IWDG_BASE_ADDR + IWDG_WIN_MIN_OFFSET)
#define IWDG_CCR_ADDR                                           ( IWDG_BASE_ADDR + IWDG_CCR_OFFSET)
#define IWDG_IRQ_ADDR                                           ( IWDG_BASE_ADDR + IWDG_IRQ_OFFSET)
#define IWDG_LOCK_ADDR                                          ( IWDG_BASE_ADDR + IWDG_LOCK_OFFSET)
#define IWDG_STATUS_ADDR                                        ( IWDG_BASE_ADDR + IWDG_STATUS_OFFSET)

// Register Field Macro Definition
#define IWDG_CTRL_DBG_STOP_EN_SHIFT                          8
#define IWDG_CTRL_DBG_STOP_EN_MASK                           0x00000100
#define IWDG_CTRL_DBG_STOP_EN_SET(n)                         (((uint32_t)(n) << 8  ) & 0x00000100)
#define IWDG_CTRL_DBG_STOP_EN_CLR                            0xFFFFFEFF

#define IWDG_CTRL_RST_EN_SHIFT                               1
#define IWDG_CTRL_RST_EN_MASK                                0x00000002
#define IWDG_CTRL_RST_EN_SET(n)                              (((uint32_t)(n) << 1  ) & 0x00000002)
#define IWDG_CTRL_RST_EN_CLR                                 0xFFFFFFFD

#define IWDG_CTRL_EN_SHIFT                                   0
#define IWDG_CTRL_EN_MASK                                    0x00000001
#define IWDG_CTRL_EN_SET(n)                                  (((uint32_t)(n) << 0  ) & 0x00000001)
#define IWDG_CTRL_EN_CLR                                     0xFFFFFFFE

#define IWDG_CNT_MAX_CNT_MAX_SHIFT                           0
#define IWDG_CNT_MAX_CNT_MAX_MASK                            0x000FFFFF
#define IWDG_CNT_MAX_CNT_MAX_SET(n)                          (((uint32_t)(n) << 0  ) & 0x000FFFFF)
#define IWDG_CNT_MAX_CNT_MAX_CLR                             0xFFF00000

#define IWDG_CCR_CCR_SHIFT                                   0
#define IWDG_CCR_CCR_MASK                                    0x00000001
#define IWDG_CCR_CCR_SET(n)                                  (((uint32_t)(n) << 0  ) & 0x00000001)
#define IWDG_CCR_CCR_CLR                                     0xFFFFFFFE

#define IWDG_IRQ_ICR_SHIFT                                   24
#define IWDG_IRQ_ICR_MASK                                    0x01000000
#define IWDG_IRQ_ICR_SET(n)                                  (((uint32_t)(n) << 24 ) & 0x01000000)
#define IWDG_IRQ_ICR_CLR                                     0xFEFFFFFF

#define IWDG_IRQ_ISR_SHIFT                                   16
#define IWDG_IRQ_ISR_MASK                                    0x00010000
#define IWDG_IRQ_ISR_SET(n)                                  (((uint32_t)(n) << 16 ) & 0x00010000)
#define IWDG_IRQ_ISR_CLR                                     0xFFFEFFFF

#define IWDG_IRQ_IRSR_SHIFT                                  8
#define IWDG_IRQ_IRSR_MASK                                   0x00000100
#define IWDG_IRQ_IRSR_SET(n)                                 (((uint32_t)(n) << 8  ) & 0x00000100)
#define IWDG_IRQ_IRSR_CLR                                    0xFFFFFEFF

#define IWDG_IRQ_IMR_SHIFT                                   0
#define IWDG_IRQ_IMR_MASK                                    0x00000001
#define IWDG_IRQ_IMR_SET(n)                                  (((uint32_t)(n) << 0  ) & 0x00000001)
#define IWDG_IRQ_IMR_CLR                                     0xFFFFFFFE

#define IWDG_LOCK_PASSWORD_SHIFT                             0
#define IWDG_LOCK_PASSWORD_MASK                              0xFFFFFFFF
#define IWDG_LOCK_PASSWORD_SET(n)                            (((uint32_t)(n) << 0  ) & 0xFFFFFFFF)
#define IWDG_LOCK_PASSWORD_CLR                               0x00000000

#define IWDG_STATUS_WCNT_SHIFT                               0
#define IWDG_STATUS_WCNT_MASK                                0x000FFFFF
#define IWDG_STATUS_WCNT_SET(n)                              (((uint32_t)(n) << 0  ) & 0x000FFFFF)
#define IWDG_STATUS_WCNT_CLR                                 0xFFF00000

// Register Structure Definition
typedef struct
{
    __IO uint32_t EN              : 1  ; // 0  : 0
    __IO uint32_t RST_EN          : 1  ; // 1  : 1
    __IO uint32_t RESERVED1       : 6  ; // 7  : 2
    __IO uint32_t DBG_STOP_EN     : 1  ; // 8  : 8
    __IO uint32_t RESERVED0       : 23 ; // 31 : 9
} IWDG_CTRL_FIELD_T;

typedef struct
{
    __IO uint32_t CNT_MAX         : 20 ; // 19 : 0
    __IO uint32_t RESERVED0       : 12 ; // 31 : 20
} IWDG_CNT_MAX_FIELD_T;

typedef struct
{
    __IO uint32_t RESERVED0       : 32 ; // 31 : 0
} IWDG_WIN_MAX_FIELD_T;

typedef struct
{
    __IO uint32_t RESERVED0       : 32 ; // 31 : 0
} IWDG_WIN_MIN_FIELD_T;

typedef struct
{
    __IO uint32_t CCR             : 1  ; // 0  : 0
    __IO uint32_t RESERVED0       : 31 ; // 31 : 1
} IWDG_CCR_FIELD_T;

typedef struct
{
    __IO uint32_t IMR             : 1  ; // 0  : 0
    __IO uint32_t RESERVED3       : 7  ; // 7  : 1
    __IO uint32_t IRSR            : 1  ; // 8  : 8
    __IO uint32_t RESERVED2       : 7  ; // 15 : 9
    __IO uint32_t ISR             : 1  ; // 16 : 16
    __IO uint32_t RESERVED1       : 7  ; // 23 : 17
    __IO uint32_t ICR             : 1  ; // 24 : 24
    __IO uint32_t RESERVED0       : 7  ; // 31 : 25
} IWDG_IRQ_FIELD_T;

typedef struct
{
    __IO uint32_t PASSWORD        : 32 ; // 31 : 0
} IWDG_LOCK_FIELD_T;

typedef struct
{
    __IO uint32_t WCNT            : 20 ; // 19 : 0
    __IO uint32_t RESERVED0       : 12 ; // 31 : 20
} IWDG_STATUS_FIELD_T;

// Register Map Structure Definition
typedef struct
{
    union
    {
        __IO  uint32_t CTRL                             ; // 0x0000
        __IO  IWDG_CTRL_FIELD_T CTRL_F                  ;
    };

    union
    {
        __IO  uint32_t CNT_MAX                          ; // 0x0004
        __IO  IWDG_CNT_MAX_FIELD_T CNT_MAX_F            ;
    };

    union
    {
        __IO  uint32_t WIN_MAX                          ; // 0x0008
        __IO  IWDG_WIN_MAX_FIELD_T WIN_MAX_F            ;
    };

    union
    {
        __IO  uint32_t WIN_MIN                          ; // 0x000C
        __IO  IWDG_WIN_MIN_FIELD_T WIN_MIN_F            ;
    };

    union
    {
        __IO  uint32_t CCR                              ; // 0x0010
        __IO  IWDG_CCR_FIELD_T CCR_F                    ;
    };

    union
    {
        __IO  uint32_t IRQ                              ; // 0x0014
        __IO  IWDG_IRQ_FIELD_T IRQ_F                    ;
    };

    union
    {
        __IO  uint32_t LOCK                             ; // 0x0018
        __IO  IWDG_LOCK_FIELD_T LOCK_F                  ;
    };

    union
    {
        __IO  uint32_t STATUS                           ; // 0x001C
        __IO  IWDG_STATUS_FIELD_T STATUS_F              ;
    };

} IWDG_REG_TypeDef;

#endif /* __IWDG_DEF_H__ */
