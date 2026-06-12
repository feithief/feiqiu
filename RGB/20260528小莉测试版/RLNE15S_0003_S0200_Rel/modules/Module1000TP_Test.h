#ifndef MODULE_1000TP_TEST_H
#define MODULE_1000TP_TEST_H

#include <stdint.h>
#include <stdbool.h>
#include "ModuleFlash.h"
#include "device.h"
#include "isr.h"

//// #include "TLD4020.h" // 替换为你的底层寄存器头文�?


#define UC_1000TP_PAGE_SIZE   (64U)
#define UC_1000TP_START       (0x11FFFDC0U)

/* BootROM 服务请求结构�?*/
typedef struct {
    uint32_t address;
    uint32_t length;
    uint8_t  data[UC_1000TP_PAGE_SIZE];
} srv_1000tp_write_t;

typedef struct {
    uint32_t address;
} srv_1000tp_erase_t;

/* 测试 API */
void Module1000TP_TestInit(void);
void Module1000TP_Erase(uint32_t page_idx);
void Module1000TP_WritePageAction(uint8_t page_index);
bool Module1000TP_TestVerify(void); // 新增：数据校验函�?
void Module1000TP_Load(ST_FLASH_CONTENT *st_FlashData);

/**
 * @brief  自适应双页连写任务核心接口
 * @note   需要在 g_u8_FlashFlag == FLASH_UPDATE_PROCESSING 的业务逻辑中调用�?
 * 该函数会根据 1000TP 的现场状态自动决定：写第1页并复位、写�?页并复位、或完成写入并结束�?
 * @return bool 如果返回 true 代表两页数据均已完美写入并对齐，写入任务彻底结束�?
 * 如果返回 false 代表触发了复位擦写，当前步骤尚未完结�?
 */
bool Module1000TP_AdaptiveWriteTask(void);
/**
 * @brief �?1000TP 区域加载配置数据到结构体�?
 * @param st_FlashData 指向目标结构体的指针
 */

#endif /* MODULE_1000TP_TEST_H */