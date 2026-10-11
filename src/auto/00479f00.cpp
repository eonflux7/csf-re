#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_008319c0_GetSubObjectField74, 0x004760A0, (C_008319c0* self, void*));

static int __fastcall C_008318c8_GetActionCategory(C_008318c8* self, void* /*edx*/)
{
    switch (C_008319c0_GetSubObjectField74(
        reinterpret_cast<C_008319c0*>(self), nullptr))
    {
    case 0:
    case 4:
        return reinterpret_cast<C_008316d8*>(self->field_0004)->field_0054 == 1
            ? 10 : 0;

    case 6:
    case 16:
    case 17:
    case 24:
    case 25:
        return 3;

    case 21:
    {
        auto* owner = reinterpret_cast<C_008316d8*>(self->field_0004);
        auto* object = reinterpret_cast<C_008316d8*>(owner->field_0044);
        auto* relatedWeapon = reinterpret_cast<C_008319c0*>(object->vSlotD8());
        if (relatedWeapon &&
            C_008319c0_GetSubObjectField74(relatedWeapon, nullptr) == 1)
            return 11;
        return 0;
    }

    case 22:
        return 12;
    case 23:
        return 13;
    case 26:
    case 27:
        return 14;
    case 28:
        return 15;
    case 31:
        return 9;
    default:
        return 0;
    }
}

HOOK(0x00479F00, C_008318c8_GetActionCategory, "auto");
