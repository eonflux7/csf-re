#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, SubmitSceneEffect, 0x0048B280, (void* self, void* edx, float* arg0));

static int __fastcall S_0048b280_SubmitEffectFromParameters(
    S_0048b280* self, void* /*edx*/, float value,
    float* parameters, float* position)
{
    alignas(8) uint8_t effectBytes[0x28];

    CSF_AT(float, effectBytes, 0x00) = value;
    CSF_AT(float, effectBytes, 0x04) = position[0];
    CSF_AT(float, effectBytes, 0x08) = position[1];
    CSF_AT(float, effectBytes, 0x0C) = position[2];
    CSF_AT(float, effectBytes, 0x10) = 0;
    CSF_AT(float, effectBytes, 0x14) = CSF_AT(float, parameters, 0x70);
    const uint32_t sharedParameter = CSF_AT(uint32_t, parameters, 0x114);
    CSF_AT(uint32_t, effectBytes, 0x20) = sharedParameter;
    CSF_AT(float, effectBytes, 0x18) = CSF_AT(float, parameters, 0x120);
    CSF_AT(float, effectBytes, 0x1C) = CSF_AT(float, parameters, 0x1A0);
    CSF_AT(uint32_t, effectBytes, 0x24) = sharedParameter;

    // The packet labels this direct-call address as a vtable reference.
    (void)CSF_VTBL(0x0048B280);
    return SubmitSceneEffect(self, nullptr, reinterpret_cast<float*>(effectBytes));
}

HOOK(0x0048B560, S_0048b280_SubmitEffectFromParameters, "auto");
