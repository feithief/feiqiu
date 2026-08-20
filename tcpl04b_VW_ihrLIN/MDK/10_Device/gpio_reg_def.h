/**
 *****************************************************************************
 * @brief   gpio_reg_def header file.
 *
 * @file    gpio_reg_def.h
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

#ifndef __GPIO_DEF_H__
#define __GPIO_DEF_H__

// Register Offset Macro Definition
#define GPIO_DATAIN_OFFSET                                      0x0000
#define GPIO_DATAOUT_OFFSET                                     0x0004
#define GPIO_OUTEN_SET_OFFSET                                   0x0008
#define GPIO_OUTEN_CLR_OFFSET                                   0x000C
#define GPIO_INT_CLR_OFFSET                                     0x0010
#define GPIO_INT_MSK_OFFSET                                     0x0014
#define GPIO_INT_POL_SEL_OFFSET                                 0x0018
#define GPIO_INT_TYP_SEL_OFFSET                                 0x001C
#define GPIO_INT_STATUS_OFFSET                                  0x0020
#define GPIO_INT_RAW_STATUS_OFFSET                              0x0024
#define GPIO_INT_EDGE_EN_OFFSET                                 0x0028

// Register Address Macro Definition
#define GPIO_DATAIN_ADDR                                        ( GPIO_BASE_ADDR + GPIO_DATAIN_OFFSET)
#define GPIO_DATAOUT_ADDR                                       ( GPIO_BASE_ADDR + GPIO_DATAOUT_OFFSET)
#define GPIO_OUTEN_SET_ADDR                                     ( GPIO_BASE_ADDR + GPIO_OUTEN_SET_OFFSET)
#define GPIO_OUTEN_CLR_ADDR                                     ( GPIO_BASE_ADDR + GPIO_OUTEN_CLR_OFFSET)
#define GPIO_INT_CLR_ADDR                                       ( GPIO_BASE_ADDR + GPIO_INT_CLR_OFFSET)
#define GPIO_INT_MSK_ADDR                                       ( GPIO_BASE_ADDR + GPIO_INT_MSK_OFFSET)
#define GPIO_INT_POL_SEL_ADDR                                   ( GPIO_BASE_ADDR + GPIO_INT_POL_SEL_OFFSET)
#define GPIO_INT_TYP_SEL_ADDR                                   ( GPIO_BASE_ADDR + GPIO_INT_TYP_SEL_OFFSET)
#define GPIO_INT_STATUS_ADDR                                    ( GPIO_BASE_ADDR + GPIO_INT_STATUS_OFFSET)
#define GPIO_INT_RAW_STATUS_ADDR                                ( GPIO_BASE_ADDR + GPIO_INT_RAW_STATUS_OFFSET)
#define GPIO_INT_EDGE_EN_ADDR                                   ( GPIO_BASE_ADDR + GPIO_INT_EDGE_EN_OFFSET)

// Register Field Macro Definition
#define GPIO_DATAIN_DATAIN_SHIFT                                    0
#define GPIO_DATAIN_DATAIN_MASK                                     0x000001FF
#define GPIO_DATAIN_DATAIN_SET(n)                                   (((uint32_t)(n) << 0  ) & 0x000001FF)
#define GPIO_DATAIN_DATAIN_CLR                                      0xFFFFFE00

#define GPIO_DATAOUT_DATAOUT_SHIFT                                  0
#define GPIO_DATAOUT_DATAOUT_MASK                                   0x000001FF
#define GPIO_DATAOUT_DATAOUT_SET(n)                                 (((uint32_t)(n) << 0  ) & 0x000001FF)
#define GPIO_DATAOUT_DATAOUT_CLR                                    0xFFFFFE00

#define GPIO_OUTEN_SET_OUTEN_SET_SHIFT                              0
#define GPIO_OUTEN_SET_OUTEN_SET_MASK                               0x000001FF
#define GPIO_OUTEN_SET_OUTEN_SET_SET(n)                             (((uint32_t)(n) << 0  ) & 0x000001FF)
#define GPIO_OUTEN_SET_OUTEN_SET_CLR                                0xFFFFFE00

#define GPIO_OUTEN_CLR_OUTEN_CLR_SHIFT                              0
#define GPIO_OUTEN_CLR_OUTEN_CLR_MASK                               0x000001FF
#define GPIO_OUTEN_CLR_OUTEN_CLR_SET(n)                             (((uint32_t)(n) << 0  ) & 0x000001FF)
#define GPIO_OUTEN_CLR_OUTEN_CLR_CLR                                0xFFFFFE00

#define GPIO_INT_CLR_INT_CLR_SHIFT                                  0
#define GPIO_INT_CLR_INT_CLR_MASK                                   0x000001FF
#define GPIO_INT_CLR_INT_CLR_SET(n)                                 (((uint32_t)(n) << 0  ) & 0x000001FF)
#define GPIO_INT_CLR_INT_CLR_CLR                                    0xFFFFFE00

#define GPIO_INT_MSK_INT_MSK_SHIFT                                  0
#define GPIO_INT_MSK_INT_MSK_MASK                                   0x000001FF
#define GPIO_INT_MSK_INT_MSK_SET(n)                                 (((uint32_t)(n) << 0  ) & 0x000001FF)
#define GPIO_INT_MSK_INT_MSK_CLR                                    0xFFFFFE00

#define GPIO_INT_POL_SEL_INT_POL_SEL_SHIFT                          0
#define GPIO_INT_POL_SEL_INT_POL_SEL_MASK                           0x000001FF
#define GPIO_INT_POL_SEL_INT_POL_SEL_SET(n)                         (((uint32_t)(n) << 0  ) & 0x000001FF)
#define GPIO_INT_POL_SEL_INT_POL_SEL_CLR                            0xFFFFFE00

#define GPIO_INT_TYP_SEL_INT_TYP_SEL_SHIFT                          0
#define GPIO_INT_TYP_SEL_INT_TYP_SEL_MASK                           0x000001FF
#define GPIO_INT_TYP_SEL_INT_TYP_SEL_SET(n)                         (((uint32_t)(n) << 0  ) & 0x000001FF)
#define GPIO_INT_TYP_SEL_INT_TYP_SEL_CLR                            0xFFFFFE00

#define GPIO_INT_STATUS_INT_STATUS_SHIFT                            0
#define GPIO_INT_STATUS_INT_STATUS_MASK                             0x000001FF
#define GPIO_INT_STATUS_INT_STATUS_SET(n)                           (((uint32_t)(n) << 0  ) & 0x000001FF)
#define GPIO_INT_STATUS_INT_STATUS_CLR                              0xFFFFFE00

#define GPIO_INT_RAW_STATUS_INT_RAW_STATUS_SHIFT                    0
#define GPIO_INT_RAW_STATUS_INT_RAW_STATUS_MASK                     0x000001FF
#define GPIO_INT_RAW_STATUS_INT_RAW_STATUS_SET(n)                   (((uint32_t)(n) << 0  ) & 0x000001FF)
#define GPIO_INT_RAW_STATUS_INT_RAW_STATUS_CLR                      0xFFFFFE00

#define GPIO_INT_EDGE_EN_INT_EDGE_EN_SHIFT                          0
#define GPIO_INT_EDGE_EN_INT_EDGE_EN_MASK                           0x000001FF
#define GPIO_INT_EDGE_EN_INT_EDGE_EN_SET(n)                         (((uint32_t)(n) << 0  ) & 0x000001FF)
#define GPIO_INT_EDGE_EN_INT_EDGE_EN_CLR                            0xFFFFFE00

// Register Structure Definition
typedef struct
{
    __IO uint32_t DATAIN          : 9  ; // 8  : 0
    __IO uint32_t RESERVED0       : 23 ; // 31 : 9
} GPIO_DATAIN_FIELD_T;

typedef struct
{
    __IO uint32_t DATAOUT         : 9  ; // 8  : 0
    __IO uint32_t RESERVED0       : 23 ; // 31 : 9
} GPIO_DATAOUT_FIELD_T;

typedef struct
{
    __IO uint32_t OUTEN_SET       : 9  ; // 8  : 0
    __IO uint32_t RESERVED0       : 23 ; // 31 : 9
} GPIO_OUTEN_SET_FIELD_T;

typedef struct
{
    __IO uint32_t OUTEN_CLR       : 9  ; // 8  : 0
    __IO uint32_t RESERVED0       : 23 ; // 31 : 9
} GPIO_OUTEN_CLR_FIELD_T;

typedef struct
{
    __IO uint32_t INT_CLR         : 9  ; // 8  : 0
    __IO uint32_t RESERVED0       : 23 ; // 31 : 9
} GPIO_INT_CLR_FIELD_T;

typedef struct
{
    __IO uint32_t INT_MSK         : 9  ; // 8  : 0
    __IO uint32_t RESERVED0       : 23 ; // 31 : 9
} GPIO_INT_MSK_FIELD_T;

typedef struct
{
    __IO uint32_t INT_POL_SEL     : 9  ; // 8  : 0
    __IO uint32_t RESERVED0       : 23 ; // 31 : 9
} GPIO_INT_POL_SEL_FIELD_T;

typedef struct
{
    __IO uint32_t INT_TYP_SEL     : 9  ; // 8  : 0
    __IO uint32_t RESERVED0       : 23 ; // 31 : 9
} GPIO_INT_TYP_SEL_FIELD_T;

typedef struct
{
    __IO uint32_t INT_STATUS      : 9  ; // 8  : 0
    __IO uint32_t RESERVED0       : 23 ; // 31 : 9
} GPIO_INT_STATUS_FIELD_T;

typedef struct
{
    __IO uint32_t INT_RAW_STATUS  : 9  ; // 8  : 0
    __IO uint32_t RESERVED0       : 23 ; // 31 : 9
} GPIO_INT_RAW_STATUS_FIELD_T;

typedef struct
{
    __IO uint32_t INT_EDGE_EN     : 9  ; // 8  : 0
    __IO uint32_t RESERVED0       : 23 ; // 31 : 9
} GPIO_INT_EDGE_EN_FIELD_T;

// Register Map Structure Definition
typedef struct
{
    union
    {
        __IO  uint32_t DATAIN                          ; // 0x0000
        __IO  GPIO_DATAIN_FIELD_T DATAIN_F             ;
    };

    union
    {
        __IO  uint32_t DATAOUT                         ; // 0x0004
        __IO  GPIO_DATAOUT_FIELD_T DATAOUT_F           ;
    };

    union
    {
        __IO  uint32_t OUTEN_SET                       ; // 0x0008
        __IO  GPIO_OUTEN_SET_FIELD_T OUTEN_SET_F       ;
    };

    union
    {
        __IO  uint32_t OUTEN_CLR                       ; // 0x000C
        __IO  GPIO_OUTEN_CLR_FIELD_T OUTEN_CLR_F       ;
    };

    union
    {
        __IO  uint32_t INT_CLR                         ; // 0x0010
        __IO  GPIO_INT_CLR_FIELD_T INT_CLR_F           ;
    };

    union
    {
        __IO  uint32_t INT_MSK                         ; // 0x0014
        __IO  GPIO_INT_MSK_FIELD_T INT_MSK_F           ;
    };

    union
    {
        __IO  uint32_t INT_POL_SEL                     ; // 0x0018
        __IO  GPIO_INT_POL_SEL_FIELD_T INT_POL_SEL_F   ;
    };

    union
    {
        __IO  uint32_t INT_TYP_SEL                     ; // 0x001C
        __IO  GPIO_INT_TYP_SEL_FIELD_T INT_TYP_SEL_F   ;
    };

    union
    {
        __IO  uint32_t INT_STATUS                      ; // 0x0020
        __IO  GPIO_INT_STATUS_FIELD_T INT_STATUS_F     ;
    };

    union
    {
        __IO  uint32_t INT_RAW_STATUS                  ; // 0x0024
        __IO  GPIO_INT_RAW_STATUS_FIELD_T INT_RAW_STATUS_F;
    };

    union
    {
        __IO  uint32_t INT_EDGE_EN                     ; // 0x0028
        __IO  GPIO_INT_EDGE_EN_FIELD_T INT_EDGE_EN_F   ;
    };

} GPIO_REG_TypeDef;

#endif /* __GPIO_DEF_H__ */
