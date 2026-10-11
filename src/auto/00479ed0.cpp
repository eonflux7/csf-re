#include "hook.h"
#include "globals.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_008319c0_GetSubObjectField74, 0x004760A0, (C_008319c0* self, void*));
CSF_FUNC(int, __fastcall, WeaponFirearms_GetDefaultState, 0x00478210, (void* self, void* edx));

static int __fastcall C_008318c8_GetState(C_008318c8* self, void* /*edx*/)
{
    const int state = C_008319c0_GetSubObjectField74(
        reinterpret_cast<C_008319c0*>(self), nullptr);

    if (state == 9)
        return 2;
    if (state == 15)
        return 0;

    return WeaponFirearms_GetDefaultState(self, nullptr);
}

HOOK(0x00479ED0, C_008318c8_GetState, "auto");
