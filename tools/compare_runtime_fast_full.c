#include <stdint.h>
#include <stdio.h>

#include "../RGB/BLINKY_SIMPLE_EXAMPLE_TLD4020/Algorithm/FloatLib.c"

#define ALGO_RATIO_K_FAST_NEAREST_ENABLE 1u
#define ALGO_RATIO_K_RUNTIME_GRID_RATIO16_ENABLE 1u
#define static
#include "../RGB/BLINKY_SIMPLE_EXAMPLE_TLD4020/Algorithm/AlgoRatioK_localfit/AlgoRatioK_localfit_ratio16_runtime.c"
#undef static

static int compare_nearest_sets(const AlgoRatioK_NearestPointType *a,
                                const AlgoRatioK_NearestPointType *b,
                                uint8_t count)
{
    uint8_t i;

    for (i = 0u; i < count; i++)
    {
        if ((a[i].lutIndex != b[i].lutIndex) ||
            (a[i].u != b[i].u) ||
            (a[i].v != b[i].v) ||
            (a[i].distance2 != b[i].distance2))
        {
            return 0;
        }
    }
    return 1;
}

int main(void)
{
    const int32_t vertexKU = (int32_t)ALGO_RATIO_K_VERTEX_K_U_Q1000;
    const int32_t vertexKV = (int32_t)ALGO_RATIO_K_VERTEX_K_V_Q1000;
    const int32_t vertexIU = (int32_t)ALGO_RATIO_K_VERTEX_I_U_Q1000;
    const int32_t vertexIV = (int32_t)ALGO_RATIO_K_VERTEX_I_V_Q1000;
    const int32_t vertexJU = (int32_t)ALGO_RATIO_K_VERTEX_J_U_Q1000;
    const int32_t vertexJV = (int32_t)ALGO_RATIO_K_VERTEX_J_V_Q1000;
    const int32_t axisIU = vertexIU - vertexKU;
    const int32_t axisIV = vertexIV - vertexKV;
    const int32_t axisJU = vertexJU - vertexKU;
    const int32_t axisJV = vertexJV - vertexKV;
    const int64_t denominator = AlgoRatioK_Cross2DS64(axisIU, axisIV, axisJU, axisJV);
    uint32_t fastFailCount = 0u;
    uint32_t fullFailCount = 0u;
    uint32_t directMismatchCount = 0u;
    uint32_t nearestMismatchCount = 0u;
    uint32_t fitMismatchCount = 0u;
    uint32_t totalCount = 0u;
    uint32_t insideCount = 0u;
    uint32_t insideFastFailCount = 0u;
    uint32_t insideFitMismatchCount = 0u;
    uint32_t firstFastFailU = 0u;
    uint32_t firstFastFailV = 0u;
    uint32_t firstFitMismatchU = 0u;
    uint32_t firstFitMismatchV = 0u;
    uint16_t targetU;
    uint16_t targetV;

    for (targetU = 49u; targetU <= 547u; targetU++)
    {
        for (targetV = 85u; targetV <= 568u; targetV++)
        {
            int32_t relU = (int32_t)targetU - vertexKU;
            int32_t relV = (int32_t)targetV - vertexKV;
            int64_t numeratorI = AlgoRatioK_Cross2DS64(relU, relV, axisJU, axisJV);
            int64_t numeratorJ = AlgoRatioK_Cross2DS64(axisIU, axisIV, relU, relV);
            int64_t numeratorK = denominator - numeratorI - numeratorJ;
            int isInside = ((numeratorI >= 0) && (numeratorJ >= 0) && (numeratorK >= 0));
            AlgoRatioK_NearestPointType fastNearest[ALGO_RATIO_K_AMOUNT];
            AlgoRatioK_NearestPointType fullNearest[ALGO_RATIO_K_AMOUNT];
            uint8_t fastCount = 0u;
            uint8_t fullCount = 0u;
            uint16_t fastDirect = 0u;
            uint16_t fullDirect = 0u;
            uint8_t fastStatus;
            uint8_t fullStatus;
            uint16_t fastRatio = 0u;
            uint16_t fullRatio = 0u;
            uint8_t fastFitStatus;
            uint8_t fullFitStatus;

            totalCount++;
            if (isInside != 0)
            {
                insideCount++;
            }

            AlgoRatioK_ResetNearestPoints(fastNearest);
            AlgoRatioK_ResetNearestPoints(fullNearest);

            fastStatus = AlgoRatioK_FindNearestPointsFast(targetU, targetV, fastNearest, &fastCount, &fastDirect);
            fullStatus = AlgoRatioK_FindNearestPointsFullScan(targetU, targetV, fullNearest, &fullCount, &fullDirect);

            if (fastStatus == 0u)
            {
                fastFailCount++;
                if (isInside != 0)
                {
                    insideFastFailCount++;
                }
                if (fastFailCount == 1u)
                {
                    firstFastFailU = targetU;
                    firstFastFailV = targetV;
                }
                continue;
            }

            if (fullStatus == 0u)
            {
                fullFailCount++;
                continue;
            }

            if ((fastStatus == 2u) || (fullStatus == 2u))
            {
                if ((fastStatus != fullStatus) || (fastDirect != fullDirect))
                {
                    directMismatchCount++;
                }
                continue;
            }

            if ((fastCount != fullCount) || (compare_nearest_sets(fastNearest, fullNearest, fastCount) == 0))
            {
                nearestMismatchCount++;
            }

            fastFitStatus = AlgoRatioK_LocalPlaneFitQ10000(fastNearest, fastCount, targetU, targetV, &fastRatio);
            fullFitStatus = AlgoRatioK_LocalPlaneFitQ10000(fullNearest, fullCount, targetU, targetV, &fullRatio);

            if (fastFitStatus == 0u)
            {
                fastFitStatus = AlgoRatioK_WeightedAverageQ10000(fastNearest, fastCount, &fastRatio);
            }
            if (fullFitStatus == 0u)
            {
                fullFitStatus = AlgoRatioK_WeightedAverageQ10000(fullNearest, fullCount, &fullRatio);
            }

            if ((fastFitStatus != fullFitStatus) || (fastRatio != fullRatio))
            {
                fitMismatchCount++;
                if (isInside != 0)
                {
                    insideFitMismatchCount++;
                }
                if (fitMismatchCount == 1u)
                {
                    firstFitMismatchU = targetU;
                    firstFitMismatchV = targetV;
                }
            }
        }
    }

    printf("total=%u\n", (unsigned)totalCount);
    printf("fast_fail=%u\n", (unsigned)fastFailCount);
    printf("full_fail=%u\n", (unsigned)fullFailCount);
    printf("direct_mismatch=%u\n", (unsigned)directMismatchCount);
    printf("nearest_mismatch=%u\n", (unsigned)nearestMismatchCount);
    printf("fit_mismatch=%u\n", (unsigned)fitMismatchCount);
    printf("inside_total=%u\n", (unsigned)insideCount);
    printf("inside_fast_fail=%u\n", (unsigned)insideFastFailCount);
    printf("inside_fit_mismatch=%u\n", (unsigned)insideFitMismatchCount);
    if (fastFailCount != 0u)
    {
        printf("first_fast_fail=(%u,%u)\n", (unsigned)firstFastFailU, (unsigned)firstFastFailV);
    }
    if (fitMismatchCount != 0u)
    {
        printf("first_fit_mismatch=(%u,%u)\n", (unsigned)firstFitMismatchU, (unsigned)firstFitMismatchV);
    }

    return 0;
}
