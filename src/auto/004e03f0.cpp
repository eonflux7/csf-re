#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Binding for the original callee (0x4DF5B0): ecx = self, one stack arg, ret 4.
CSF_FUNC(void*, __fastcall, sub_4DF5B0, 0x004DF5B0, (void* self, void* edx, int a2));

// Resets the S_004e03f0 record: zeroes the embedded counter block at +0x0C
// (last dword set to 10), clears the flag/value fields, pads the counter up to
// 1000 via sub_4DF5B0 when it is below 1000, then sets the limit to 1000 and
// the float field to FLT_MAX. Returns the struct address.
static uint32_t* __fastcall S_004e03f0_InitDefaults(S_004e03f0* self, void* /*edx*/)
{
    uint8_t* counterBlock = (uint8_t*)self + 0x0C;

    CSF_AT(uint32_t, counterBlock, 0) = 0;
    CSF_AT(uint32_t, counterBlock, 4) = 0;
    CSF_AT(uint32_t, counterBlock, 8) = 0;
    CSF_AT(uint32_t, counterBlock, 12) = 10;

    self->field_0020 = 0;
    self->field_0024 = 0;
    self->field_0028 = 0;
    self->field_0000 = 0;
    self->field_002c = 0;

    uint32_t currentCount = CSF_AT(uint32_t, counterBlock, 4);
    if (currentCount < 1000u)
    {
        sub_4DF5B0(counterBlock, nullptr, (int)(1000u - currentCount));
    }

    self->field_0018 = 1000;
    self->field_0008 = 2139095039u;
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x4E03F0, S_004e03f0_InitDefaults, "auto");
