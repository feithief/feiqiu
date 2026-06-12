#ifndef MODULE_DEBUG_H_
#define MODULE_DEBUG_H_

#include "stdint.h"
#include "device.h"
#include "isr.h"
#include "ModuleFlash.h" /* 假设 g_st_SavedConfig 定义在此处 */

#define LED_MIN_BRIGHTNESS 0x0000
#define LED_MAX_BRIGHTNESS 0x0FFF

/* ========================================================================== */
/* UDS 协议指令映射宏定义                                                     */
/* ========================================================================== */
/* XX: 激活服务的魔术字 (例如使用 0x5A 作为激活标志，防止误触发) */
#define UDS_CTRL_MAGIC_WORD  0x5A  

/* YY: 接口类型定义 */
#define UDS_TARGET_SWD       0x01
#define UDS_TARGET_BSL       0x02

/* ZZ: 操作类型定义 */
#define UDS_OP_DISABLE       0x00
#define UDS_OP_ENABLE        0x01

/* ========================================================================== */
/* 接口操作类型与结构体定义                                                   */
/* ========================================================================== */

/**
 * @brief The operations of interface management, either activate or deactivate the selected interface.
 */
typedef enum
{
  IF_DEACTIVATE     = 0x55u,    /**< deactivate SWD access */
  IF_ACTIVATE       = 0xFFu     /**< activate SWD access */
} interface_op_t;

/**
 * @brief interface selection, used by interface management operation.
 */
typedef enum
{
  SWD     = 0x01u,    /**< select target interface SWD */
  BSL     = 0x10u     /**< select target interface BSL */
} interface_t;

/**
 * @brief data structure of service request operation: interface management.
 */
typedef struct
{
  interface_t interface;
  interface_op_t interface_op;
  uint16_t reserved; /* padding */
} srv_ifmgnt_t;

/* ========================================================================== */
/* 模块对外提供的函数声明                                                     */
/* ========================================================================== */

/**
 * \brief 系统复位状态检查与 BootROM 服务清理初始化
 */
void System_Startup_Check(void);

/**
 * \brief 轮询 UDS 接口控制请求
 */
void Process_UDS_Interface_Control(void);

/**
 * \brief 底层通用接口控制触发
 */
void Device_ControlInterface(uint8_t interface, uint8_t interface_op);

#endif /* MODULE_DEBUG_H_ */