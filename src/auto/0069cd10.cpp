#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, sub_69CBF0, 0x0069CBF0, (void* self, void* edx));

CSF_GLOBAL(void*, g_QueryPerformanceCounterIat, 0x0082D0C0);
CSF_GLOBAL(void*, g_TimeGetTimeIat, 0x0082D324);

static uint8_t* __fastcall C_0085e92c_Construct(
    C_0085e92c* self, void* /*edx*/, int /*unused*/)
{
    self->vtbl = CSF_VTBL(0x0085E92C);
    self->field_003c = 0;
    self->field_003d = 1;
    sub_69CBF0(self, nullptr);

    uint32_t lowPart;
    uint32_t highPart;

    if (self->field_003c)
    {
        const uint64_t timestamp = __builtin_ia32_rdtsc();
        lowPart = static_cast<uint32_t>(timestamp);
        highPart = static_cast<uint32_t>(timestamp >> 32);
    }
    else if (self->field_003d)
    {
        alignas(8) uint8_t counterBytes[0x8] = {};

        // Keep the object in ECX at the import boundary. The two register
        // parameters add no stack arguments; the import still pops four bytes.
        reinterpret_cast<int(__fastcall*)(void*, void*, void*)>(
            g_QueryPerformanceCounterIat)(self, nullptr, counterBytes);

        lowPart = CSF_AT(uint32_t, counterBytes, 0);
        highPart = CSF_AT(uint32_t, counterBytes, 4);
    }
    else
    {
        lowPart = reinterpret_cast<uint32_t(__fastcall*)(void*, void*)>(
            g_TimeGetTimeIat)(self, nullptr);
        highPart = 0;
    }

    self->field_0008 = lowPart;
    self->field_0010 = lowPart;
    self->field_0030 = 0;
    self->field_0034 = 0;
    self->field_0038 = 0;
    self->field_0020 = 0;
    self->field_000c = highPart;
    self->field_0014 = highPart;

    return reinterpret_cast<uint8_t*>(self);
}

HOOK(0x0069CD10, C_0085e92c_Construct, "auto");
