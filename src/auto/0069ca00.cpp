#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// IAT slots: the game calls through these pointers (stdcall, one LARGE_INTEGER* argument).
CSF_GLOBAL(void*, g_QueryPerformanceCounterIat, 0x82D0C0);
// IAT slot for timeGetTime (stdcall, no arguments, returns DWORD).
CSF_GLOBAL(void*, g_TimeGetTimeIat, 0x82D324);

// Refreshes the object's three timestamp pairs (+0x08/+0x0C, +0x10/+0x14, +0x28/+0x2C)
// from the best available clock and returns the low 32 bits of the timestamp.
static uint32_t __fastcall C_0085e92c_RefreshTimestamp(C_0085e92c* self, void* /*edx*/)
{
    uint32_t timeLow = 0;
    uint32_t timeHigh = 0;

    if (self->field_003c) {
        // Cycle counter (rdtsc).
        uint64_t cycles = __rdtsc();
        timeLow = static_cast<uint32_t>(cycles);
        timeHigh = static_cast<uint32_t>(cycles >> 32);
    }
    else if (self->field_003d) {
        // High-resolution performance counter: 8 bytes on the caller's stack.
        alignas(8) uint8_t counterBytes[8];
        ((int(__stdcall*)(void*))g_QueryPerformanceCounterIat)(counterBytes);
        timeLow = reinterpret_cast<uint32_t*>(counterBytes)[0];
        timeHigh = reinterpret_cast<uint32_t*>(counterBytes)[1];
    }
    else {
        // Millisecond clock.
        timeLow = ((uint32_t(__stdcall*)())g_TimeGetTimeIat)();
        timeHigh = 0;
    }

    self->field_0008 = timeLow;
    self->field_000c = timeHigh;
    self->field_0010 = timeLow;
    self->field_0014 = timeHigh;
    self->field_0028 = timeLow;
    self->field_002c = timeHigh;
    return timeLow;
}

HOOK(0x69CA00, C_0085e92c_RefreshTimestamp, "auto");
