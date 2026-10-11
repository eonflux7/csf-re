#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, CharActionFsm_InitializeHitInfo, 0x0040ED90, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(unsigned int, __fastcall, S_0051c1b0_GrowEntries, 0x0051C1B0, (void* self, void* edx, int newCapacity));
CSF_FUNC(double, __cdecl, RwV3dLength, 0x00623360, (float* a1));
CSF_FUNC(int, __fastcall, SceneLoader_GetSceneController, 0x00652010, (void* self, void* edx));
CSF_FUNC(float*, __fastcall, SceneMissionSetup_GetBoundsCenter, 0x006AB660, (void* self, void* edx, float* a2));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __cdecl, FxParticleManager_GetParticleController, 0x006CA810, (int a1));
CSF_FUNC(int, __fastcall, FxParticleManager_UpdateScaledParameters, 0x006DE030, (void* self, void* edx));

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(float, g_MinimumParticleSpan, 0x00840F1C);
CSF_GLOBAL(float, g_ParticleSpanScale, 0x00840F44);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static void __fastcall C_00840f28_SpawnScaledParticles(
    C_00840f28* self, void* /*edx*/, uint32_t* event, S_0051c1b0* entries)
{
    if (event[1] != 40)
        return;

    void* source = reinterpret_cast<void*>(event[13]);
    void* owner = nullptr;
    void* savedSource = source;

    if (static_cast<int32_t>(event[4]) < 0) {
        owner = ((void* (__fastcall*)(void*, void*))
            CSF_AT(void**, source, 0)[28 / 4])(source, nullptr);
    } else {
        void* resolved = reinterpret_cast<void*>(
            CHandleTable_Resolve(g_pCurrentScene, nullptr,
                reinterpret_cast<int>(event + 2)));
        if (resolved) {
            owner = ((void* (__fastcall*)(void*, void*))
                CSF_AT(void**, resolved, 0)[40 / 4])(resolved, nullptr);
            if (owner) {
                savedSource = ((void* (__fastcall*)(void*, void*))
                    CSF_AT(void**, owner, 0)[120 / 4])(owner, nullptr);
                source = savedSource;
            }
        }
    }

    if (!source)
        return;
    if (!((bool (__fastcall*)(void*, void*))
        CSF_AT(void**, source, 0)[40 / 4])(source, nullptr))
        return;

    // Retain the original contiguous scratch layout: several outputs share
    // storage with scalar locals.
    alignas(8) uint8_t scratch[0x120];
    void* hitInfo = scratch;
    float* matrix = reinterpret_cast<float*>(scratch + 0x68);
    float* segment = reinterpret_cast<float*>(scratch + 0x94);
    void* secondBounds = scratch + 0xAC;
    float* center = reinterpret_cast<float*>(scratch + 0xB8);
    void* firstBounds = scratch + 0xCC;
    float* displacement = reinterpret_cast<float*>(scratch + 0xD8);
    int32_t* entryHandle = reinterpret_cast<int32_t*>(scratch + 0xF4);
    float* sourcePosition = reinterpret_cast<float*>(scratch + 0xFC);
    float* origin = reinterpret_cast<float*>(scratch + 0x108);

    void* sceneController = reinterpret_cast<void*>(
        SceneLoader_GetSceneController(
            reinterpret_cast<void*>(GetGlobalInt()), nullptr));

    void* transform = ((void* (__fastcall*)(void*, void*))
        CSF_AT(void**, owner, 0)[24 / 4])(owner, nullptr);
    float* position = ((float* (__fastcall*)(void*, void*))
        CSF_AT(void**, transform, 0)[16 / 4])(transform, nullptr);

    origin[0] = position[0];
    origin[1] = position[1];
    origin[2] = position[2];

    void* ownerBounds = ((void* (__fastcall*)(void*, void*))
        CSF_AT(void**, owner, 0)[40 / 4])(owner, nullptr);
    float extent = CSF_AT(float, ownerBounds, 0xC);

    center[0] = origin[0];
    center[1] = origin[1] + extent;
    center[2] = origin[2];
    displacement[0] = origin[0];
    displacement[1] = origin[1] - extent;
    displacement[2] = origin[2];

    segment[0] = origin[0];
    segment[1] = center[1];
    segment[2] = origin[2];
    segment[3] = origin[0];
    segment[4] = displacement[1];
    segment[5] = origin[2];

    void* collisionController = ((void* (__fastcall*)(void*, void*))
        CSF_AT(void**, sceneController, 0)[60 / 4])(sceneController, nullptr);
    void* hit = ((void* (__fastcall*)(void*, void*, int, float*, int))
        CSF_AT(void**, collisionController, 0)[56 / 4])(
            collisionController, nullptr, 0, segment, 3);

    if (hit) {
        CharActionFsm_InitializeHitInfo(hitInfo, nullptr);
        ((void (__fastcall*)(void*, void*, void*))
            CSF_AT(void**, hit, 0)[12 / 4])(hit, nullptr, hitInfo);

        void* gameState = reinterpret_cast<void*>(GetValue8B0554());
        void* settings = CSF_AT(void*, gameState, 0x1FDC);
        float heightLimit = CSF_AT(float, settings, 0x30);

        void* boundsA = ((void* (__fastcall*)(void*, void*, void*))
            CSF_AT(void**, source, 0)[60 / 4])(source, nullptr, firstBounds);
        void* boundsB = ((void* (__fastcall*)(void*, void*, void*))
            CSF_AT(void**, source, 0)[60 / 4])(source, nullptr, secondBounds);

        float threshold = heightLimit - g_HalfDurationScale *
            (CSF_AT(float, boundsB, 0x4) - CSF_AT(float, boundsA, 0x10));
        if (CSF_AT(float, hitInfo, 0x8) > threshold)
            return;
    }

    float* spanVector = ((float* (__fastcall*)(void*, void*, float*))
        CSF_AT(void**, source, 0)[80 / 4])(source, nullptr, displacement);
    double span = RwV3dLength(spanVector);
    CSF_AT(float, scratch, 0xC8) = static_cast<float>(span);
    if (span < g_MinimumParticleSpan)
        return;

    CSF_AT(void**, scratch, 0xE8) = CSF_VTBL(0x0082D448);
    CSF_AT(int32_t, scratch, 0xEC) = -1;
    CSF_AT(int32_t, scratch, 0xF0) = -1;
    CSF_AT(int, scratch, 0xE4) = 0;
    entryHandle[0] = -1;
    entryHandle[1] = -1;

    void* definitions = CSF_AT(void*,
        reinterpret_cast<void*>(GetValue8B0554()), 0x1FCC);
    CSF_AT(void*, scratch, 0xC4) = definitions;
    uint32_t index = 0;
    if (!self->field_0010)
        return;

    do {
        entryHandle[1] = reinterpret_cast<int32_t*>(self->field_0008)[index];
        void* definition = ((void* (__fastcall*)(void*, void*, int))
            CSF_AT(void**, definitions, 0)[8 / 4])(
                definitions, nullptr, entryHandle[1]);

        void* factory = CSF_AT(void*, self, 0x4);
        int effect = ((int (__fastcall*)(void*, void*, void*, int, int32_t*))
            CSF_AT(void**, factory, 0)[8 / 4])(
                factory, nullptr, static_cast<uint8_t*>(definition) + 4,
                10, entryHandle);
        CSF_AT(int, scratch, 0xE4) = effect;

        if (effect) {
            void* particleController = reinterpret_cast<void*>(
                FxParticleManager_GetParticleController(effect));
            void* bounds = ((void* (__fastcall*)(void*, void*, float*))
                CSF_AT(void**, source, 0)[60 / 4])(
                    source, nullptr, sourcePosition);

            displacement[0] = sourcePosition[0] - origin[0];
            displacement[1] = sourcePosition[1] - origin[1];
            displacement[2] = sourcePosition[2] - origin[2];
            SceneMissionSetup_GetBoundsCenter(bounds, nullptr, center);

            void* particleTransform = ((void* (__fastcall*)(void*, void*))
                CSF_AT(void**, particleController, 0)[36 / 4])(
                    particleController, nullptr);
            std::memcpy(matrix,
                static_cast<uint8_t*>(particleTransform) + 212, 0x2C);
            matrix[4] = (displacement[2] + displacement[0]) *
                g_HalfDurationScale * g_HalfDurationScale;

            ((void (__fastcall*)(void*, void*, float*, int))
                CSF_AT(void**, particleController, 0)[28 / 4])(
                    particleController, nullptr, matrix, 1);

            float scale = CSF_AT(float, scratch, 0xC8) * g_ParticleSpanScale;
            void* parameters = ((void* (__fastcall*)(void*, void*))
                CSF_AT(void**, particleController, 0)[44 / 4])(
                    particleController, nullptr);

            int64_t scaledCount = static_cast<int64_t>(
                static_cast<double>(CSF_AT(int32_t, parameters, 0xC)) * scale);
            float scaledDuration = scale * CSF_AT(float, parameters, 0x24);
            CSF_AT(uint32_t, parameters, 0xC) = static_cast<uint32_t>(scaledCount);
            CSF_AT(float, parameters, 0x24) = scaledDuration;
            CSF_AT(float, parameters, 0xB4) =
                scale * CSF_AT(float, parameters, 0xB4);
            CSF_AT(float, parameters, 0xB8) =
                scale * CSF_AT(float, parameters, 0xB8);
            CSF_AT(float, parameters, 0x18) =
                scale * CSF_AT(float, parameters, 0x18);
            FxParticleManager_UpdateScaledParameters(parameters, nullptr);

            center[1] -= displacement[1] * g_HalfDurationScale;
            void* spawnedEffect = reinterpret_cast<void*>(
                CSF_AT(int, scratch, 0xE4));
            ((void (__fastcall*)(void*, void*, float*))
                CSF_AT(void**, spawnedEffect, 0)[56 / 4])(
                    spawnedEffect, nullptr, center);
            ((void (__fastcall*)(void*, void*, int))
                CSF_AT(void**, spawnedEffect, 0)[40 / 4])(
                    spawnedEffect, nullptr, 0);

            if (entries->field_0008 >= entries->field_0004)
                S_0051c1b0_GrowEntries(entries, nullptr, entries->field_000c);

            uint32_t entryIndex = entries->field_0008;
            source = savedSource;
            int32_t* destination = reinterpret_cast<int32_t*>(
                entries->field_0000 + 24 * entryIndex);
            entries->field_0008 = entryIndex + 1;
            definitions = CSF_AT(void*, scratch, 0xC4);
            destination[0] = CSF_AT(int, scratch, 0xE4);
            destination[2] = CSF_AT(int32_t, scratch, 0xEC);
            destination[3] = CSF_AT(int32_t, scratch, 0xF0);
            destination[4] = entryHandle[0];
            destination[5] = entryHandle[1];
        }
        ++index;
    } while (index < self->field_0010);
}

HOOK(0x0051E0A0, C_00840f28_SpawnScaledParticles, "auto");
