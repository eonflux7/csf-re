#include "hook.h"
#include "globals.h"
#include "types/auto_classes.h"

static CSF_FUNC(int, __fastcall, C_008319c0_GetSubObjectField74, 0x004760A0, (C_008319c0* self, void*));
static CSF_FUNC(bool, __fastcall, C_008318c8_CheckFireEligibility, 0x00476780, (void* self, void* edx));

static bool __fastcall C_008318c8_CanFire(C_008318c8* self, void* /*edx*/)
{
    return C_008319c0_GetSubObjectField74(reinterpret_cast<C_008319c0*>(self), nullptr) != 31
        && C_008319c0_GetSubObjectField74(reinterpret_cast<C_008319c0*>(self), nullptr) != 32
        && C_008319c0_GetSubObjectField74(reinterpret_cast<C_008319c0*>(self), nullptr) != 23
        && C_008319c0_GetSubObjectField74(reinterpret_cast<C_008319c0*>(self), nullptr) != 27
        && (C_008319c0_GetSubObjectField74(reinterpret_cast<C_008319c0*>(self), nullptr) != 28
            || self->field_0080 != 0)
        && C_008318c8_CheckFireEligibility(self, nullptr);
}

HOOK(0x00479E70, C_008318c8_CanFire, "auto");
