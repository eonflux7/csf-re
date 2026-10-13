#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
CSF_FUNC(void, __fastcall, C_00862748_UpdateParticles_6F4590, 0x006F4590, (C_00862748* self, void*));
CSF_FUNC(int, __fastcall, SceneLoader_GetObjectRegistry, 0x00726C40, (void* self, void* edx));

CSF_GLOBAL(float, g_PositionHeightOffset, 0x0082DDE8);
CSF_GLOBAL(float, g_ProgressScale, 0x0082E03C);
CSF_GLOBAL(float, g_ParticleDistanceScale, 0x008627D0);

static unsigned int __fastcall C_00862748_UpdateAtomic(
    C_00862748* self, void* /*edx*/)
{
    // This path returns the entire EAX from the virtual call, although
    // only AL determines whether the update is skipped.
    const unsigned int flagResult =
        reinterpret_cast<unsigned int (__fastcall*)(C_00862748*, void*)>(
            CSF_AT(void**, self, 0)[0x34 / 4])(self, nullptr);
    if (static_cast<uint8_t>(flagResult))
        return flagResult;

    float threshold = g_ProgressScale;
    void* associatedObject = reinterpret_cast<void*>(self->field_0018);
    if (!associatedObject || !CSF_AT(uint8_t, associatedObject, 0x41))
    {
        void* globalValues =
            reinterpret_cast<void*>(GetGlobalValue_6C5760());
        uint32_t* position = self->vGetGlobal8BB420();

        const float dx =
            CSF_AT(float, globalValues, 0x124) - CSF_AT(float, position, 0);
        const float dy =
            CSF_AT(float, globalValues, 0x128) - CSF_AT(float, position, 4);
        const float dz =
            CSF_AT(float, globalValues, 0x12C) - CSF_AT(float, position, 8);

        globalValues = reinterpret_cast<void*>(GetGlobalValue_6C5760());
        const float distanceScale = CSF_AT(float, globalValues, 0x130);
        void* particleSettings = reinterpret_cast<void*>(self->field_0030);

        float distanceThreshold =
            (dz * dz + dy * dy + dx * dx) * distanceScale * distanceScale *
            g_ParticleDistanceScale + g_ProgressScale;

        if (particleSettings)
        {
            distanceThreshold =
                distanceThreshold /
                (CSF_AT(float, particleSettings, 0x94) +
                 CSF_AT(float, particleSettings, 0x24) +
                 g_PositionHeightOffset) *
                g_PositionHeightOffset;
        }

        void* sceneLoader = reinterpret_cast<void*>(GetGlobalInt());
        void* registry = reinterpret_cast<void*>(
            SceneLoader_GetObjectRegistry(sceneLoader, nullptr));
        const int quality =
            reinterpret_cast<int (__fastcall*)(void*, void*)>(
                CSF_AT(void**, registry, 0)[0x54 / 4])(registry, nullptr);

        const int qualityMultiplier =
            static_cast<int>(3u - static_cast<unsigned int>(quality));
        threshold = static_cast<float>(qualityMultiplier) * distanceThreshold;
    }

    if (threshold < self->field_0040)
        C_00862748_UpdateParticles_6F4590(self, nullptr);

    using AtomicCallback = void (__cdecl*)(uint32_t);
    const uint32_t atomic = self->field_0024;
    CSF_AT(AtomicCallback, reinterpret_cast<void*>(atomic), 0x48)(atomic);

    const unsigned int result = self->field_0014 & 0xFFFFFFF7;
    self->field_0014 = result;
    return result;
}

HOOK(0x006F4F40, C_00862748_UpdateAtomic, "auto");
