#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

using QueryPerformanceFn = int (__stdcall*)(void*);
using GetCurrentThreadFn = void* (__stdcall*)();
using GetThreadPriorityFn = int (__stdcall*)(void*);
using SetThreadPriorityFn = int (__stdcall*)(void*, int);

CSF_GLOBAL(QueryPerformanceFn, g_QueryPerformanceFrequencyIat, 0x82D0C4);
CSF_GLOBAL(QueryPerformanceFn, g_QueryPerformanceCounterIat, 0x82D0C0);
CSF_GLOBAL(GetCurrentThreadFn, g_GetCurrentThreadIat, 0x82D24C);
CSF_GLOBAL(GetThreadPriorityFn, g_GetThreadPriorityIat, 0x82D254);
CSF_GLOBAL(SetThreadPriorityFn, g_SetThreadPriorityIat, 0x82D208);

static std::uint32_t __cdecl GfxRenderEngine_MeasureCpuFrequencyMHz()
{
    alignas(8) std::uint8_t frequencyBytes[0x8];
    alignas(8) std::uint8_t performanceCountBytes[0x14];

    const int result = g_QueryPerformanceFrequencyIat(frequencyBytes);
    if (!result)
        return static_cast<std::uint32_t>(result);

    void* currentThread = g_GetCurrentThreadIat();
    const int originalPriority = g_GetThreadPriorityIat(currentThread);
    g_SetThreadPriorityIat(currentThread, 15);

    const QueryPerformanceFn queryCounter = g_QueryPerformanceCounterIat;
    queryCounter(performanceCountBytes);
    const std::uint32_t startingCycles =
        static_cast<std::uint32_t>(__builtin_ia32_rdtsc());
    const std::uint32_t startingCount =
        CSF_AT(std::uint32_t, performanceCountBytes, 0);

    std::uint32_t endingCycles;
    do
    {
        queryCounter(performanceCountBytes);
        endingCycles = static_cast<std::uint32_t>(__builtin_ia32_rdtsc());
    }
    while (CSF_AT(std::uint32_t, performanceCountBytes, 0) - startingCount
           < 0x2710);

    g_SetThreadPriorityIat(currentThread, originalPriority);

    const std::uint32_t elapsedCount =
        CSF_AT(std::uint32_t, performanceCountBytes, 0) - startingCount;
    const std::uint32_t scaledElapsed =
        (10000u * elapsedCount) /
        (CSF_AT(std::uint32_t, frequencyBytes, 0) / 0x64);

    return (endingCycles - startingCycles) / scaledElapsed;
}

HOOK(0x6DC670, GfxRenderEngine_MeasureCpuFrequencyMHz, "auto");
