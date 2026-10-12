#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);
CSF_GLOBAL(float, g_TransitionThreshold, 0x00860194);

CSF_FUNC(int, __fastcall, C_00860288_SetVectorImmediate, 0x006B3780,
         (void* self, void* edx, float* a2));
CSF_FUNC(uint32_t*, __fastcall, C_00860288_ReadVector, 0x006B40D0,
         (void* self, void* edx, uint32_t* a2));

static uint32_t* __fastcall C_00860288_SetVectorTransition(
    C_00860288* self, void* /*edx*/, float* target, float duration)
{
    const bool immediate = duration <= g_TransitionThreshold;

    self->field_009c = CSF_AT(uint32_t, target, 0x00);
    self->field_00a0 = CSF_AT(uint32_t, target, 0x04);
    self->field_00a4 = CSF_AT(uint32_t, target, 0x08);
    CSF_AT(uint32_t, self, 0xA8) = CSF_AT(uint32_t, target, 0x0C);
    *reinterpret_cast<float*>(&self->field_00ac) = duration;

    if (immediate)
    {
        return reinterpret_cast<uint32_t*>(
            C_00860288_SetVectorImmediate(self, nullptr, target));
    }

    alignas(8) uint8_t currentBytes[0x10];
    uint32_t* result = C_00860288_ReadVector(
        self, nullptr, reinterpret_cast<uint32_t*>(currentBytes));

    const float inverseDuration =
        g_TransitionOne / *reinterpret_cast<float*>(&self->field_00ac);

    CSF_AT(float, self, 0xB0) =
        (*reinterpret_cast<float*>(&self->field_009c) -
         CSF_AT(float, currentBytes, 0x00)) * inverseDuration;
    CSF_AT(float, self, 0xB4) =
        (*reinterpret_cast<float*>(&self->field_00a0) -
         CSF_AT(float, currentBytes, 0x04)) * inverseDuration;
    CSF_AT(float, self, 0xB8) =
        (*reinterpret_cast<float*>(&self->field_00a4) -
         CSF_AT(float, currentBytes, 0x08)) * inverseDuration;

    return result;
}

HOOK(0x006B45D0, C_00860288_SetVectorTransition, "auto");
