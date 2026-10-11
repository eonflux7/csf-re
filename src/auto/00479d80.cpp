#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static int __fastcall C_008318c8_GetEffectiveSecondaryWeaponValue(
    C_008318c8* self, void* /*edx*/)
{
    if (CSF_AT(uint8_t, reinterpret_cast<void*>(self->field_0004), 0x228))
        return static_cast<int>(self->field_002c);

    return static_cast<int>(self->field_0084);
}

HOOK(0x00479D80, C_008318c8_GetEffectiveSecondaryWeaponValue, "auto");
