#include "ModuleDebug.h"

/* ========================================================================== */
/* 函数实现                                                                   */
/* ========================================================================== */

/**
 * \brief 系统复位状态检查与 BootROM 服务清理初始化
 * \note  必须在 main 函数的 Device_init() 之后调用
 */
void System_Startup_Check(void)
{
		moduleWatchdogFeed();
    if (PMU->RESETSTS.bit.VDD1V5UV)
    {
        /* 清除欠压复位标志位 */
        PMU->RESETSC.bit.VDD1V5UV = 1; 
    }

    if (PMU->RESETSTS.bit.SOFT)
    {
        /* 清除软复位标志位 */
        PMU->RESETSC.bit.SOFT = 1; 
				
    }

    /* 检查上一次由软复位触发的 BootROM 服务例程是否执行出错 */
    if (MEM->SRVREQ.bit.LASTREQ && MEM->SRVREQ.bit.RESULTSTS)
    {
        /* RED LED: 服务执行失败 */
        LDRV->PWM_DC_CFG0.reg = LED_MAX_BRIGHTNESS;
    }

    /* 清理服务请求寄存器配置，使其恢复干净状态 */
    MEM->SRVREQ.reg = 0;
    MEM->SRVREQPTR.reg = 0;
		
//    /* 诊断全局变量初始化安全防御 (防止 RAM 保持导致的无限复位死循环) */
//    g_st_SavedConfig.u8_DebugSwitch[0] = 0x00;
//    g_st_SavedConfig.u8_DebugSwitch[1] = 0x00;
//    g_st_SavedConfig.u8_DebugSwitch[2] = 0x00;
}

/**
 * \brief  轮询 UDS 接口控制请求，并执行相应的封装函数
 */
void Process_UDS_Interface_Control(void)
{
    /* 1. 判断是否收到魔术字激活请求 */
    if (g_st_SavedConfig.u8_DebugSwitch[0] == UDS_CTRL_MAGIC_WORD)
    {
        uint8_t target_interface = g_st_SavedConfig.u8_DebugSwitch[1];
        uint8_t target_operation = g_st_SavedConfig.u8_DebugSwitch[2];

        /* ⚠️ 关键步骤：执行底层复位操作前必须先清零数组，打断死循环条件 */
        g_st_SavedConfig.u8_DebugSwitch[0] = 0x00;
        g_st_SavedConfig.u8_DebugSwitch[1] = 0x00;
        g_st_SavedConfig.u8_DebugSwitch[2] = 0x00;

        /* 2. 执行对应操作 */
        switch (target_interface)
        {
            case UDS_TARGET_SWD:
                if (target_operation == UDS_OP_DISABLE)
                {
                    Device_ControlInterface((uint8_t)SWD, (uint8_t)IF_DEACTIVATE);
                }
                else if (target_operation == UDS_OP_ENABLE)
                {
                    Device_ControlInterface((uint8_t)SWD, (uint8_t)IF_ACTIVATE);
                }
                break;

            case UDS_TARGET_BSL:
                if (target_operation == UDS_OP_DISABLE)
                {
                    Device_ControlInterface((uint8_t)BSL, (uint8_t)IF_DEACTIVATE);
                }
                else if (target_operation == UDS_OP_ENABLE)
                {
                    Device_ControlInterface((uint8_t)BSL, (uint8_t)IF_ACTIVATE);
                }
                break;

            default:
                break;
        }
    }
}

/**
 * \brief  安全接口配置通用函数（执行后会触发芯片软复位）
 */
void Device_ControlInterface(uint8_t interface, uint8_t interface_op)
{
    
    srv_ifmgnt_t srv_ifmgnt;

    /* 将 uint8_t 强转为你定义的枚举类型 */
    srv_ifmgnt.interface    = interface;
    srv_ifmgnt.interface_op = interface_op;
    srv_ifmgnt.reserved     = 0;

    /* 准备启动服务例程请求 */
	MEM->SRVREQ.reg = 0u;//debug
    MEM->SRVREQPTR.reg = (uint32_t)&srv_ifmgnt;
    MEM->SRVREQ.bit.IFMNGT = 1;


    /* 清除复位状态寄存器 */
    PMU->RESETSC.reg = 0xFFFFFFFF;

    /* 触发软复位，执行 BootROM 服务 */
//    __NVIC_SystemReset();
		NVIC_SystemReset();
}



