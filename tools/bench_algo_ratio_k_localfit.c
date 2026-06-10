#include <stdint.h>
#include <stdio.h>
#include <windows.h>

#include "AlgoRatioK_localfit.h"

#ifndef BENCH_TARGET_COUNT
#define BENCH_TARGET_COUNT (4096u)
#endif

#ifndef BENCH_REPEAT_COUNT
#define BENCH_REPEAT_COUNT (64u)
#endif

typedef struct
{
    uint16_t u;
    uint16_t v;
} BenchTargetType;

static uint32_t bench_lcg_next(uint32_t *state)
{
    *state = (*state * 1664525u) + 1013904223u;
    return *state;
}

static void bench_build_targets(BenchTargetType *targets, uint32_t count)
{
    uint32_t state = 0x13572468u;
    uint32_t index;

    for (index = 0u; index < count; index++)
    {
        uint32_t uSpan = 547u - 49u;
        uint32_t vSpan = 568u - 85u;
        targets[index].u = (uint16_t)(49u + (bench_lcg_next(&state) % (uSpan + 1u)));
        targets[index].v = (uint16_t)(85u + (bench_lcg_next(&state) % (vSpan + 1u)));
    }
}

static double bench_seconds_now(void)
{
    LARGE_INTEGER freq;
    LARGE_INTEGER counter;

    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&counter);
    return (double)counter.QuadPart / (double)freq.QuadPart;
}

int main(void)
{
    static BenchTargetType targets[BENCH_TARGET_COUNT];
    volatile uint32_t checksum = 0u;
    uint32_t repeatIndex;
    uint32_t targetIndex;
    uint32_t okCount = 0u;
    double startTime;
    double endTime;
    double totalSeconds;
    double avgMicroseconds;

    bench_build_targets(targets, BENCH_TARGET_COUNT);

    startTime = bench_seconds_now();
    for (repeatIndex = 0u; repeatIndex < BENCH_REPEAT_COUNT; repeatIndex++)
    {
        for (targetIndex = 0u; targetIndex < BENCH_TARGET_COUNT; targetIndex++)
        {
            uint16_t whiteRatioQ10000 = 0u;
            uint8_t status = AlgoRatioK_GetWhiteRatioQ10000(targets[targetIndex].u,
                                                            targets[targetIndex].v,
                                                            &whiteRatioQ10000);
            okCount += (uint32_t)status;
            checksum += ((uint32_t)whiteRatioQ10000 + (uint32_t)status);
        }
    }
    endTime = bench_seconds_now();

    totalSeconds = endTime - startTime;
    avgMicroseconds = (totalSeconds * 1000000.0) / (double)(BENCH_TARGET_COUNT * BENCH_REPEAT_COUNT);

    printf("runtime_grid_ratio16=%u\n", (unsigned)ALGO_RATIO_K_RUNTIME_GRID_RATIO16_ENABLE);
    printf("fast_nearest=%u\n", (unsigned)ALGO_RATIO_K_FAST_NEAREST_ENABLE);
    printf("target_count=%u\n", (unsigned)BENCH_TARGET_COUNT);
    printf("repeat_count=%u\n", (unsigned)BENCH_REPEAT_COUNT);
    printf("total_calls=%u\n", (unsigned)(BENCH_TARGET_COUNT * BENCH_REPEAT_COUNT));
    printf("ok_count=%u\n", (unsigned)okCount);
    printf("checksum=%u\n", (unsigned)checksum);
    printf("total_seconds=%.6f\n", totalSeconds);
    printf("avg_microseconds=%.3f\n", avgMicroseconds);

    return 0;
}
