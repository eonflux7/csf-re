#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// IAT cells: the original calls through these pointers.
CSF_GLOBAL(void*, g_QueryPerformanceCounterIat, 0x82D0C0);
CSF_GLOBAL(void*, g_TimeGetTimeIat, 0x82D324);

// Start/stop stopwatch. Samples a clock (TSC, QueryPerformanceCounter or
// timeGetTime, chosen by flags at +0x3C/+0x3D), then either starts a measurement
// or stops it and adds the elapsed span to the 64-bit accumulator at +0x08.
static int __fastcall C_0085e92c_ToggleStopwatch(C_0085e92c* self, void* /*edx*/)
{
    uint32_t lowPart;
    uint32_t highPart;

    // Room for a LARGE_INTEGER passed to QueryPerformanceCounter.
    alignas(8) uint32_t counterWords[2];

    if (self->field_003c) {
        uint64_t cycles = __builtin_readcyclecounter();
        lowPart = (uint32_t)cycles;
        highPart = (uint32_t)(cycles >> 32);
    } else if (self->field_003d) {
        reinterpret_cast<void(__stdcall*)(void*)>(g_QueryPerformanceCounterIat)(counterWords);
        lowPart = counterWords[0];
        highPart = counterWords[1];
    } else {
        lowPart = reinterpret_cast<uint32_t(__cdecl*)()>(g_TimeGetTimeIat)();
        highPart = 0;
    }

    uint64_t sample = ((uint64_t)highPart << 32) | lowPart;

    if (self->field_0020) {
        uint64_t start = ((uint64_t)self->field_002c << 32) | self->field_0028;
        uint64_t total = ((uint64_t)self->field_000c << 32) | self->field_0008;
        total = total + (sample - start);
        self->field_0008 = (uint32_t)total;
        self->field_000c = (uint32_t)(total >> 32);
        self->field_0020 = 0;
        self->field_0010 = lowPart;
        self->field_0014 = highPart;
    } else {
        self->field_0020 = 1;
        self->field_0028 = lowPart;
        self->field_002c = highPart;
    }

    return (int)lowPart;
}

HOOK(0x0069CB30, C_0085e92c_ToggleStopwatch, "auto");
