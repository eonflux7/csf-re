#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
CSF_FUNC(bool, __cdecl, GfxPrtStdParticles_ComputeDistanceFade, 0x006F3D10, (float* position, float* fade));

static char __fastcall C_008622e8_SubmitParticleTransform(
    C_008622e8* self, void* /*edx*/)
{
    if (self->vCheckGlobalObjectFlag())
        return 0;

    if (self->field_0024) {
        float* fade = reinterpret_cast<float*>(
            reinterpret_cast<uint8_t*>(self) + 0x34);
        float* position =
            reinterpret_cast<float*>(self->vGetGlobal8BB420());
        if (!GfxPrtStdParticles_ComputeDistanceFade(position, fade))
            return 0;
    }

    void* transformObject =
        reinterpret_cast<uint8_t*>(self->field_0020) + 0x20;
    const void* source =
        (reinterpret_cast<const void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, transformObject, 0)[0x8 / 4]))(
                transformObject, nullptr);

    const uint32_t applyPositionOffset = self->field_0024;
    alignas(8) uint8_t transformBytes[0x54] = {};
    std::memcpy(transformBytes, source, 0x3C);
    float* transform = reinterpret_cast<float*>(transformBytes);

    if (applyPositionOffset) {
        float* parentPosition = reinterpret_cast<float*>(
            reinterpret_cast<C_00860a30*>(self->field_0020)
                ->vGetGlobal8BB420());

        const float offsetX = CSF_AT(float, self, 0x28) - parentPosition[0];
        const float offsetY = CSF_AT(float, self, 0x2C) - parentPosition[1];
        const float offsetZ = CSF_AT(float, self, 0x30) - parentPosition[2];
        transform[0] += offsetX;
        transform[1] += offsetY;
        transform[2] += offsetZ;
    }

    int objectFlag = 0;
    if (reinterpret_cast<C_00860a30*>(self->field_0020)->vSlot1C()) {
        void* object = reinterpret_cast<void*>(
            reinterpret_cast<C_00860a30*>(self->field_0020)->vSlot1C());
        if (CSF_AT(uint8_t, object, 0x41))
            objectFlag = 1;
    }

    void* globalObject = reinterpret_cast<void*>(GetGlobalValue_6C5760());
    void* renderer = CSF_AT(void*, globalObject, 0xF0);
    void* particleData = reinterpret_cast<uint8_t*>(self) + 0x38;

    return static_cast<char>(
        (reinterpret_cast<int (__fastcall*)(void*, void*, float*, void*, int)>(
            CSF_AT(void**, renderer, 0)[0x74 / 4]))(
                renderer, nullptr, transform, particleData, objectFlag));
}

HOOK(0x006F07B0, C_008622e8_SubmitParticleTransform, "auto");
