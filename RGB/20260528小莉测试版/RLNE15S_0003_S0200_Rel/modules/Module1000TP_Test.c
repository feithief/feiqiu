#include "Module1000TP_Test.h"

#define MODULE1000TP_LASTREQ_ERASE  (0x02u)

static uint8_t g_u8_1000tpLastReq = 0u;
static uint8_t g_u8_1000tpLastResult = 0u;


/**
 * @brief 测试初始�?
 */
void Module1000TP_TestInit(void)
{
    g_u8_1000tpLastReq = (uint8_t)MEM->SRVREQ.bit.LASTREQ;
    g_u8_1000tpLastResult = (uint8_t)MEM->SRVREQ.bit.RESULTSTS;
    /* 清除冷启动标�?*/
    if (PMU->RESETSTS.bit.VDD1V5UV) {
        PMU->RESETSC.bit.VDD1V5UV = 1; 
    }
    
    /* 清除软复位标志，并检查上一�?BootROM 操作是否报错 */
    if (PMU->RESETSTS.bit.SOFT) {
        PMU->RESETSC.bit.SOFT = 1; 
        
        if (MEM->SRVREQ.bit.LASTREQ && MEM->SRVREQ.bit.RESULTSTS) {
            /* 如果你接了红灯，可以在这里点亮代�?BootROM 报错 */
            // LDRV->PWM_DC_CFG0.reg = LED_MAX_BRIGHTNESS; 
        }
    }
    
    MEM->SRVREQ.reg = 0;
    MEM->SRVREQPTR.reg = 0;
}

/**
 * @brief 擦除1000TP指定�?
 * @param page_idx 页号（从0开始）
 */
void Module1000TP_Erase(uint32_t page_idx)
{
    srv_1000tp_erase_t g_srv_1000tp_erase;

    g_srv_1000tp_erase.address = UC_1000TP_START +
                                  page_idx * UC_1000TP_PAGE_SIZE;

    MEM->SRVREQ.reg = 0u;
    MEM->SRVREQPTR.reg = (uint32_t)&g_srv_1000tp_erase;
    MEM->SRVREQ.bit.ER1000TP = 1u;

    /* 清除复位标志 */
    PMU->RESETSC.reg = 0xFFFFFFFF;
    __NVIC_SystemReset();
    while (1) { }
}



/**
 * @brief  通用�?1000TP 单页物理写入执行函数
 * @param  page_index : 页码�? 代表写第 1 页（�?64 字节），1 代表写第 2 页（剩余字节�?
 * @warning 调用后系统会立即触发 BootROM 服务并执行热复位重启�?
 */
void Module1000TP_WritePageAction(uint8_t page_index)
{
    uint8_t *p_source = (uint8_t *)&g_st_SavedConfig;
    
    /* 局部变量：承袭原厂设计，利用栈天然落入高地址 RAM 区域，避开�?1KB 冲突 */
    srv_1000tp_write_t srv_1000tp_write; 
    
    // 1. 计算 1000TP 的目标物理基地址
    // page_index = 0 -> UC_1000TP_START
    // page_index = 1 -> UC_1000TP_START + 64
    srv_1000tp_write.address = UC_1000TP_START + (page_index * UC_1000TP_PAGE_SIZE);
    
    // 2. 根据页码动态计算写入长度和源数据内存偏�?
    uint32_t source_offset = page_index * UC_1000TP_PAGE_SIZE;
    
    srv_1000tp_write.length = UC_1000TP_PAGE_SIZE;

    /* 3. 1000TP page write must always receive one full 64-byte page. */
    for(uint32_t i = 0; i < UC_1000TP_PAGE_SIZE; i++)
    {
        if ((source_offset + i) < sizeof(ST_FLASH_CONTENT))
        {
            srv_1000tp_write.data[i] = p_source[source_offset + i];
        }
        else
        {
            srv_1000tp_write.data[i] = 0xFFu;
        }
    }
    
    /* 4. 将局部变量的物理地址传给寄存器并触发 BootROM 动作 */
    MEM->SRVREQ.reg = 0u;
    MEM->SRVREQPTR.reg = (uint32_t)&srv_1000tp_write;
    MEM->SRVREQ.bit.WR1000TP = 1u;
    
    /* 5. 触发软复位前，清空所有历史复位标志状�?*/
    PMU->RESETSC.reg = 0xFFFFFFFF;
    __NVIC_SystemReset(); 
    while (1) { }
}

/**
 * @brief 校验函数：利用供应商提供的异�?XOR)算法比对数据
 * @retval true  校验通过 (1000TP 的数据与预期一�?
 * @retval false 校验失败
 */
bool Module1000TP_TestVerify(void)
{
    uint32_t u32_expected_xor_checksum = 0;
    uint32_t u32_actual_xor_checksum = 0;
    uint8_t* u8p_1000tp_read_data = (uint8_t *)UC_1000TP_START;

    /* 1. 计算【预期写入数据】的 Checksum (刚才我们写的�?0~63) */
    for (uint32_t i = 0; i < UC_1000TP_PAGE_SIZE; i++)
    {
        u32_expected_xor_checksum ^= (uint32_t)((uint8_t)i) << ((i & 3) * 8);
    }

    /* 2. 计算【Flash中实际数据】的 Checksum */
    for (uint32_t i = 0; i < UC_1000TP_PAGE_SIZE; i++)
    {
        u32_actual_xor_checksum ^= (uint32_t)u8p_1000tp_read_data[i] << ((i & 3) * 8);
    }

    /* 3. 对比并返回结�?*/
    if (u32_expected_xor_checksum == u32_actual_xor_checksum)
    {
        /* 校验成功，如果板子上有绿灯可以在这亮�?*/
        // LDRV->PWM_DC_CFG2.reg = LED_MAX_BRIGHTNESS;
        return true;
    }
    
    return false;
}


/**
 * @brief �?1000TP 区域加载配置数据到结构体�?
 */
void Module1000TP_Load(ST_FLASH_CONTENT *st_FlashData)
{
    uint16_t u16_FlashDataSize = sizeof(ST_FLASH_CONTENT);
    uint16_t u16_Addr = 0;
    
    // 1000TP 的直接内存映射基地址指针
    uint8_t *u8p_1000tp_base = (uint8_t *)UC_1000TP_START;
    
    // �?1000TP 内存中逐字节读取并存入 st_FlashData 结构�?
    for(u16_Addr = 0; u16_Addr < u16_FlashDataSize; u16_Addr++)
    {
        // 1000TP 是标准的内存映射区，直接用指针偏移读取即可，不需要调 ByteRead 函数
        *((uint8_t*)st_FlashData + u16_Addr) = *(u8p_1000tp_base + u16_Addr);
    }
    
    if ((g_st_SavedConfig.u16_LedSupplier & 0x00f0u) ==  0x0080u)
    {	
		// Set the flash locked flag
		g_u8_FlashFlag |= (uint8_t)FLASH_UPDATE_LOCKED;

    }
    else
    {
		// Clear the flash locked flag
        g_u8_FlashFlag &= ~((uint8_t)FLASH_UPDATE_LOCKED);
    }
}



static bool Module1000TP_LastEraseSucceeded(void)
{
    bool ret = false;

    if (((g_u8_1000tpLastReq & MODULE1000TP_LASTREQ_ERASE) != 0u) && (g_u8_1000tpLastResult == 0u))
    {
        ret = true;
    }

    return ret;
}
bool Module1000TP_AdaptiveWriteTask(void)
{
    uint8_t *p_tp_page1 = (uint8_t *)UC_1000TP_START;
    uint8_t *p_tp_page2 = (uint8_t *)(UC_1000TP_START + UC_1000TP_PAGE_SIZE);
    uint8_t *p_ram_base = (uint8_t *)&g_st_SavedConfig;

    uint32_t u32_TotalSize = sizeof(ST_FLASH_CONTENT); // 112 字节
    uint32_t u32_Page2Len  = u32_TotalSize - UC_1000TP_PAGE_SIZE; // 48 字节

	    /* ------------------------------------------------------------------------
     * 【清�?2】：后检查第二页 (64 ~ 111 字节)
     * 如果第一页完全一致（没有被上面的 if 拦截断流），代码才会走到这里�?
     * 此时严格对比第二页的变量，只要对不上，就去写第二页！
     * ------------------------------------------------------------------------ */
    bool b_Page2NeedUpdate = false;
    for (uint32_t i = 0; i < u32_Page2Len; i++)
    {
        if (p_tp_page2[i] != p_ram_base[UC_1000TP_PAGE_SIZE + i])
        {
            b_Page2NeedUpdate = true;
            break;
        }
    }

    if (b_Page2NeedUpdate)
    {

        if (Module1000TP_LastEraseSucceeded() == true)
        {
            Module1000TP_WritePageAction(1);
        }
        else
        {
            Module1000TP_Erase(1);
        }
        return false; 
    }
	
    /* ------------------------------------------------------------------------
     * 【清�?1】：先检查第一�?(0 ~ 63 字节)
     * 顺应变量顺序，先看第一页的关键参数。只要对不上，立刻去写第一页！
     * ------------------------------------------------------------------------ */
    bool b_Page1NeedUpdate = false;
    for (uint32_t i = 0; i < UC_1000TP_PAGE_SIZE; i++)
    {
        if (p_tp_page1[i] != p_ram_base[i]) 
        { 
            b_Page1NeedUpdate = true; 
            break; 
        }
    }

    if (b_Page1NeedUpdate)
    {
        if (Module1000TP_LastEraseSucceeded() == true)
        {
            Module1000TP_WritePageAction(0);
        }
        else
        {
            Module1000TP_Erase(0);
        }
        return false; 
    }

    /* ------------------------------------------------------------------------
     * 【清�?3】：两页全对齐，通关
     * ------------------------------------------------------------------------ */
    return true; 
}