#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static char __fastcall C_008316d8_UpdateStateFromWeapon(
    C_008316d8* self, void* /*edx*/, char preserveState)
{
    self->field_001c = CSF_AT(
        uint32_t, reinterpret_cast<void*>(self->field_0040), 0x104);

    if (!preserveState)
    {
        return reinterpret_cast<char (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, self, 0)[0x78 / 4])(self, nullptr, 1);
    }
    return preserveState;
}

HOOK(0x00475C80, C_008316d8_UpdateStateFromWeapon, "auto");
