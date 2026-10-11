#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall C_008670f0_IsActiveBeyondLimits(C_008670f0* self, void* /*edx*/)
{
    void* owner = reinterpret_cast<void*>(self->field_0004);
    if (!reinterpret_cast<bool (__fastcall*)(void*, void*)>(
            CSF_AT(void**, owner, 0)[0xC0 / 4])(owner, nullptr))
        return 0;

    if (self->field_00c8)
        return 1;

    alignas(8) uint8_t firstLimitsBytes[0x8];
    alignas(8) uint8_t secondLimitsBytes[0x8];

    uint32_t* limits = self->vSlot5C(
        reinterpret_cast<uint32_t*>(firstLimitsBytes));
    if (static_cast<int32_t>(self->field_00d0) >
        static_cast<int32_t>(limits[1]))
        return 1;

    limits = self->vSlot5C(reinterpret_cast<uint32_t*>(secondLimitsBytes));
    return static_cast<int32_t>(self->field_00d4) >
           static_cast<int32_t>(limits[0]) ? 1 : 0;
}

HOOK(0x00732C20, C_008670f0_IsActiveBeyondLimits, "auto");
