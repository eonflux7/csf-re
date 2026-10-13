#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, FillBufferToFiveEntries, 0x00518DA0, (void* self, void* edx, int a2));

static uint32_t* __fastcall S_00518f00_Init(S_00518f00* self, void* /*edx*/, int a2)
{
    uint8_t* pad = (uint8_t*)self + 0xC;

    CSF_AT(uint32_t, pad, 0x0) = 0;
    CSF_AT(uint32_t, pad, 0x4) = 0;
    CSF_AT(uint32_t, pad, 0x8) = 0;
    CSF_AT(uint32_t, pad, 0xC) = 10;

    self->field_0000 = static_cast<uint32_t>(a2);
    self->field_0004 = 0;
    self->field_0008 = 0;

    unsigned int filledCount = CSF_AT(uint32_t, pad, 0x4);
    if (filledCount < 5)
        FillBufferToFiveEntries(pad, nullptr, static_cast<int>(5 - filledCount));

    return &self->field_0000;
}

HOOK(0x00518F00, S_00518f00_Init, "auto");
