#include <stdio.h>
#include <stdint.h>

// 多项式除数 0xEDB88320
#define POLY 0xEDB88320UL

// 计算 CRC32 校验和
uint32_t crc32(const void *data, size_t len)
{
    const uint8_t *bytes = data;
    uint32_t u16_Crc = 0xFFFFFFFFUL;

    // 循环处理每个字节
    for (size_t i = 0; i < len; i++) {
        u16_Crc ^= bytes[i];        // 把当前字节与 u16_Crc 的低 8 位进行异或操作

        // 处理当前字节的 8 位，每次处理一位
        for (int j = 0; j < 8; j++) {
            if (u16_Crc & 1) {      // 如果 u16_Crc 的最低位为 1，则jian右移并与多项式除数进行异或操作
                u16_Crc = (u16_Crc >> 1) ^ POLY;
            } else {            // 否则，只右移一个比特位
                u16_Crc >>= 1;
            }
        }
    }

    return ~u16_Crc;                // 取反操作得到最终结果
}


