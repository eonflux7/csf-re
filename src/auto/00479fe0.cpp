#include "hook.h"
#include "globals.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_008319c0_GetSubObjectField74, 0x004760A0, (C_008319c0* self, void*));

static int __fastcall C_008318c8_GetWeaponCategory(C_008318c8* self, void* /*edx*/)
{
    switch (C_008319c0_GetSubObjectField74(reinterpret_cast<C_008319c0*>(self), nullptr))
    {
    case 9:
        return 5;
    case 10:
        return 6;
    case 15:
        return 8;
    default:
        return 0;
    }
}

HOOK(0x00479FE0, C_008318c8_GetWeaponCategory, "auto");
