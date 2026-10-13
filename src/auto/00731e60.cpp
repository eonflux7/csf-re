#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, sub_7317E0, 0x007317E0, (void* self, void* edx, int* a2, float a3));
CSF_FUNC(int, __fastcall, CanConfigureEffect, 0x007319C0, (void* self, void* edx, int effect));

static bool __fastcall IsQuadEffectActive(C_00867078* self, void* /*edx*/, int* a2)
{
    if (!a2)
        return false;

    // Preferred configuration: effect slot 1, full strength.
    int configA = CanConfigureEffect(self, nullptr, 1);
    if (configA && CSF_AT(uint8_t, reinterpret_cast<void*>(configA), 4))
    {
        sub_7317E0(self, nullptr, a2, 1.0f);
        return true;
    }

    // Fallback configuration: effect slot 0, strength from the virtual slot 4.
    int configB = CanConfigureEffect(self, nullptr, 0);
    if (!configB || !CSF_AT(uint8_t, reinterpret_cast<void*>(configB), 4))
        return false;

    float strength = static_cast<float>(self->vSlot4());
    sub_7317E0(self, nullptr, a2, strength);
    return true;
}

HOOK(0x731E60, IsQuadEffectActive, "auto");
