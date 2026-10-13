#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Kernel32 QueryPerformanceCounter, reached through its IAT slot.
typedef int(__stdcall* QueryPerformanceCounterFn)(int64_t* counter);
// winmm timeGetTime, reached through its IAT slot.
typedef unsigned int(__stdcall* TimeGetTimeFn)(void);

CSF_GLOBAL(void*, g_QueryPerformanceCounterIat, 0x82D0C0);
CSF_GLOBAL(void*, g_TimeGetTimeIat, 0x82D324);

static uint64_t __fastcall C_0085e92c_ReadCurrentTickCounter(C_0085e92c* self, void* /*edx*/)
{
    if (self->field_003c)
    {
        // Hardware time-stamp counter.
        return __builtin_ia32_rdtsc();
    }
    if (self->field_003d)
    {
        // High-resolution performance counter.
        int64_t performanceCount = 0;
        reinterpret_cast<QueryPerformanceCounterFn>(g_QueryPerformanceCounterIat)(&performanceCount);
        return static_cast<uint64_t>(performanceCount);
    }
    // Fallback: millisecond tick count, zero-extended to 64 bits.
    return static_cast<uint64_t>(reinterpret_cast<TimeGetTimeFn>(g_TimeGetTimeIat)());
}

HOOK(0x0069C9B0, C_0085e92c_ReadCurrentTickCounter, "auto");
