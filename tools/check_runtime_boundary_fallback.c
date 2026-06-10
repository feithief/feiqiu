#include <stdint.h>
#include <stdio.h>
#include "AlgoRatioK_localfit.h"

int main(void)
{
    uint32_t failCount = 0u;
    uint32_t totalCount = 0u;
    uint16_t firstFailU = 0u;
    uint16_t firstFailV = 0u;
    uint16_t u;
    uint16_t v;

    for (u = 49u; u <= 547u; u++)
    {
        for (v = 85u; v <= 568u; v++)
        {
            uint16_t ratio = 0u;
            uint8_t ok;
            totalCount++;
            ok = AlgoRatioK_GetWhiteRatioQ10000(u, v, &ratio);
            if (ok == 0u)
            {
                failCount++;
                if (failCount == 1u)
                {
                    firstFailU = u;
                    firstFailV = v;
                }
            }
        }
    }

    printf("total=%u\n", (unsigned)totalCount);
    printf("fail=%u\n", (unsigned)failCount);
    if (failCount != 0u)
    {
        printf("first_fail=(%u,%u)\n", (unsigned)firstFailU, (unsigned)firstFailV);
    }
    return 0;
}
