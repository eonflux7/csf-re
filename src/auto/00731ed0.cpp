#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CanConfigureEffect, 0x007319C0, (void* self, void* edx, int effect));
CSF_FUNC(void, __fastcall, ApplyQuadEffectParams, 0x007317E0, (void* self, void* edx, int* a2, float a3));

static bool __fastcall C_00867078_IsQuadHandledByEffect(C_00867078* self, void* /*edx*/, int* a2)
{
    if (!a2)
        return false;

    int effectHandle = CanConfigureEffect(self, nullptr, 1);
    if (!effectHandle || !CSF_AT(uint8_t, reinterpret_cast<uint8_t*>(effectHandle), 4))
        return false;

    float slot8Value = static_cast<float>(self->vSlot8());
    ApplyQuadEffectParams(self, nullptr, a2, slot8Value);
    return true;
}

HOOK(0x731ED0, C_00867078_IsQuadHandledByEffect, "auto");
