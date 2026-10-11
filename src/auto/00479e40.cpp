#include "hook.h"
#include "globals.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_008319c0_GetSubObjectField74, 0x004760A0, (C_008319c0* self, void*));
CSF_FUNC(bool, __fastcall, WeaponFirearms_IsFireReady, 0x004767B0, (void* self, void* edx));

static bool __fastcall C_008318c8_IsFireReadyWithStateRestriction(
    C_008318c8* self, void* /*edx*/)
{
    const int state = C_008319c0_GetSubObjectField74(
        reinterpret_cast<C_008319c0*>(self), nullptr);
    const bool ready = WeaponFirearms_IsFireReady(self, nullptr);

    if (self->field_007c)
        return ready && state != 0 && state != 4;

    return ready;
}

HOOK(0x00479E40, C_008318c8_IsFireReadyWithStateRestriction, "auto");
