/**
 *****************************************************************************
 * @brief   print_uart_reg_def header file.
 *
 * @file    print_uart_reg_def.h
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

#ifndef __PRINT_UART_DEF_H__
#define __PRINT_UART_DEF_H__

// Register Offset Macro Definition
#define PRINT_UART_TX_DATA_OFFSET                               0x0000
#define PRINT_UART_PRESCALE_OFFSET                              0x0004
#define PRINT_UART_STATUS_OFFSET                                0x0008

// Register Address Macro Definition
#define PRINT_UART_TX_DATA_ADDR                                 ( PRINT_UART_BASE_ADDR + PRINT_UART_TX_DATA_OFFSET)
#define PRINT_UART_PRESCALE_ADDR                                ( PRINT_UART_BASE_ADDR + PRINT_UART_PRESCALE_OFFSET)
#define PRINT_UART_STATUS_ADDR                                  ( PRINT_UART_BASE_ADDR + PRINT_UART_STATUS_OFFSET)

// Register Field Macro Definition
#define PRINT_UART_TX_DATA_TX_DATA_SHIFT                            0
#define PRINT_UART_TX_DATA_TX_DATA_MASK                             0x000000FF
#define PRINT_UART_TX_DATA_TX_DATA_SET(n)                           (((uint32_t)(n) << 0  ) & 0x000000FF)
#define PRINT_UART_TX_DATA_TX_DATA_CLR                              0xFFFFFF00

#define PRINT_UART_PRESCALE_PRESCALE_SHIFT                          0
#define PRINT_UART_PRESCALE_PRESCALE_MASK                           0x00003FFF
#define PRINT_UART_PRESCALE_PRESCALE_SET(n)                         (((uint32_t)(n) << 0  ) & 0x00003FFF)
#define PRINT_UART_PRESCALE_PRESCALE_CLR                            0xFFFFC000

#define PRINT_UART_STATUS_TX_BUSY_SHIFT                             0
#define PRINT_UART_STATUS_TX_BUSY_MASK                              0x00000001
#define PRINT_UART_STATUS_TX_BUSY_SET(n)                            (((uint32_t)(n) << 0  ) & 0x00000001)
#define PRINT_UART_STATUS_TX_BUSY_CLR                               0xFFFFFFFE

// Register Structure Definition
typedef struct
{
    __IO uint32_t TX_DATA         : 8  ; // 7  : 0
    __IO uint32_t RESERVED0       : 24 ; // 31 : 8
} PRINT_UART_TX_DATA_FIELD_T;

typedef struct
{
    __IO uint32_t PRESCALE        : 14 ; // 13 : 0
    __IO uint32_t RESERVED0       : 18 ; // 31 : 14
} PRINT_UART_PRESCALE_FIELD_T;

typedef struct
{
    __IO uint32_t TX_BUSY         : 1  ; // 0  : 0
    __IO uint32_t RESERVED0       : 31 ; // 31 : 1
} PRINT_UART_STATUS_FIELD_T;

// Register Map Structure Definition
typedef struct
{
    union
    {
        __IO  uint32_t TX_DATA                         ; // 0x0000
        __IO  PRINT_UART_TX_DATA_FIELD_T TX_DATA_F     ;
    };

    union
    {
        __IO  uint32_t PRESCALE                        ; // 0x0004
        __IO  PRINT_UART_PRESCALE_FIELD_T PRESCALE_F   ;
    };

    union
    {
        __IO  uint32_t STATUS                          ; // 0x0008
        __IO  PRINT_UART_STATUS_FIELD_T STATUS_F       ;
    };

} PRINT_UART_REG_TypeDef;

#endif /* __PRINT_UART_DEF_H__ */
