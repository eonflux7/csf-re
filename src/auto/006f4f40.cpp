#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
CSF_FUNC(void, __fastcall, C_00862748_UpdateParticles, 0x006F4590, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_GetObjectRegistry, 0x00726C40, (void* self, void* edx));

CSF_GLOBAL(float, g_PositionHeightOffset, 0x0082DDE8);
CSF_GLOBAL(float, g_ProgressScale, 0x0082E03C);
CSF_GLOBAL(float, g_ParticleDistanceScale, 0x008627D0);

static uint32_t __fastcall C_00862748_UpdateAtomic(
    C_00862748* self, void* /*edx*/)
{
    uint32_t result = static_cast<uint8_t>(self->vCheckGlobalObjectFlag());
    if (result != 0)
        return result;

    void* owner = reinterpret_cast<void*>(self->field_0018);
    float updateThreshold = g_ProgressScale;

    if (!owner || !CSF_AT(uint8_t, owner, 0x41))
    {
        void* globals = reinterpret_cast<void*>(GetGlobalValue_6C5760());
        float* position = reinterpret_cast<float*>(self->vGetGlobal8BB420());

        float dx = CSF_AT(float, globals, 0x124) - position[0];
        float dy = CSF_AT(float, globals, 0x128) - position[1];
        float dz = CSF_AT(float, globals, 0x12C) - position[2];

        void* currentGlobals = reinterpret_cast<void*>(GetGlobalValue_6C5760());
        float distanceScale = CSF_AT(float, currentGlobals, 0x130);
        S_006dcf90* emitter =
            reinterpret_cast<S_006dcf90*>(self->field_0030);

        float interval = (dz * dz + dy * dy + dx * dx)
            * distanceScale * distanceScale * g_ParticleDistanceScale
            + g_ProgressScale;

        if (emitter)
        {
            float extent94;
            float extent24;
            std::memcpy(&extent94, &emitter->field_0094, sizeof(extent94));
            std::memcpy(&extent24, &emitter->field_0024, sizeof(extent24));
            interval = interval
                / (extent94 + extent24 + g_PositionHeightOffset)
                * g_PositionHeightOffset;
        }

        void* sceneLoader = reinterpret_cast<void*>(GetGlobalInt());
        void* registry = reinterpret_cast<void*>(
            SceneLoader_GetObjectRegistry(sceneLoader, nullptr));

        int level =
            (reinterpret_cast<int(__fastcall*)(void*, void*)>(
                CSF_AT(void**, registry, 0)[0x54 / 4]))(registry, nullptr);

        int32_t multiplier =
            static_cast<int32_t>(3u - static_cast<uint32_t>(level));
        updateThreshold = static_cast<float>(multiplier) * interval;
    }

    if (updateThreshold < self->field_0040)
        C_00862748_UpdateParticles(self, nullptr);

    using AtomicUpdateCallback = void(__cdecl*)(uint32_t);
    uint32_t atomic = self->field_0024;
    AtomicUpdateCallback updateAtomic = CSF_AT(
        AtomicUpdateCallback, reinterpret_cast<void*>(atomic), 0x48);
    updateAtomic(atomic);

    result = self->field_0014 & 0xFFFFFFF7;
    self->field_0014 = result;
    return result;
}

HOOK(0x006F4F40, C_00862748_UpdateAtomic, "auto");
