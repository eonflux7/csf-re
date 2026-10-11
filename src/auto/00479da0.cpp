#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static int __fastcall C_008318c8_GetEffectiveWeaponParameter(
    C_008318c8* self, void* /*edx*/)
{
    if (CSF_AT(uint8_t, reinterpret_cast<void*>(self->field_0004), 0x228))
        return static_cast<int>(self->field_0028);

    return static_cast<int>(self->field_0088);
}

HOOK(0x00479DA0, C_008318c8_GetEffectiveWeaponParameter, "auto");
