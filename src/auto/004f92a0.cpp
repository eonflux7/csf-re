#include <cstdint>
#include <cmath>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(float*, __fastcall, BuildFacingVectorBetween, 0x00402030, (float* self, void* , float* a2, float* a3));
CSF_FUNC(int, __fastcall, GfxMatrix_AreVectorsDifferent, 0x0040FB20, (float* self, void* , float* other));
CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(double, __cdecl, RwV3dLength, 0x00623360, (float* a1));
CSF_FUNC(float*, __fastcall, NormalizeVector3, 0x006AB8D0, (C_00833b88* self, void*));

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_ProximityEpsilon, 0x0083F1D4);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_GLOBAL(float, g_ZeroVectorX, 0x008BB420);

static void __fastcall DevConsole_UpdateProximityIntensity(
    void* self, void* /*edx*/, float deltaTime)
{
    void* context = reinterpret_cast<void*>(GetSceneMenuContext(g_pCurrentScene, nullptr));
    if (!context)
        return;

    float* intensity = &CSF_AT(float, self, 0x0C);
    const float previousIntensity = *intensity;

    if (CSF_AT(float, self, 0x08) > g_fZero)
        CSF_AT(float, self, 0x08) -= deltaTime;
    else
        *intensity -= deltaTime * CSF_AT(float, self, 0x14) * g_HalfDurationScale;

    if (*intensity < g_ProximityEpsilon)
        *intensity = 0.0f;

    if (!CSF_AT(uint8_t, CSF_AT(void*, g_pCurrentScene, 0xC0), 0x09))
    {
        // Preserve the original frame's room and overlapping vector/event storage.
        alignas(8) uint8_t scratch[0x40];
        float* eventData = reinterpret_cast<float*>(scratch);
        float* facingResult = reinterpret_cast<float*>(scratch + 0x04);
        float* displacement = reinterpret_cast<float*>(scratch + 0x10);
        float* nearestDirection = reinterpret_cast<float*>(scratch + 0x1C);

        float* origin = GetObjectPositionVector(context, nullptr);
        nearestDirection[0] = 0.0f;
        nearestDirection[1] = 0.0f;
        nearestDirection[2] = 0.0f;
        float nearestDistance = CSF_AT(float, self, 0x18);
        const uint32_t objectCount = CSF_AT(uint32_t, g_pCurrentScene, 0x18);

        for (uint32_t index = 0; index < objectCount; ++index)
        {
            void* object = CSF_AT(void**, g_pCurrentScene, 0x10)[index];
            if (!object)
                continue;

            if (((bool (__fastcall*)(void*, void*))
                    CSF_AT(void**, object, 0)[0x7C / 4])(object, nullptr))
                continue;

            const int objectType = CSF_AT(int, object, 0x50);
            if (objectType != 5 && objectType != 4)
                continue;

            float* position = GetObjectPositionVector(object, nullptr);
            displacement[2] = position[2] - origin[2];
            displacement[1] = position[1] - origin[1];
            displacement[0] = position[0] - origin[0];
            const float distance = static_cast<float>(RwV3dLength(displacement));

            if (!(distance < CSF_AT(float, self, 0x18)))
                continue;

            void* innerObject = CSF_AT(void*, object, 0x30);
            if (((bool (__fastcall*)(void*, void*))
                    CSF_AT(void**, innerObject, 0)[0xB8 / 4])(innerObject, nullptr))
                continue;

            float candidateIntensity;
            if (distance <= CSF_AT(float, self, 0x1C))
            {
                candidateIntensity = CSF_AT(float, self, 0x20)
                    - distance * CSF_AT(float, self, 0x28);
            }
            else
            {
                candidateIntensity = CSF_AT(float, self, 0x24)
                    - (CSF_AT(float, self, 0x1C) - distance)
                        * CSF_AT(float, self, 0x2C);
            }

            *intensity = *intensity >= candidateIntensity ? *intensity : candidateIntensity;

            if (nearestDistance > distance)
            {
                position = GetObjectPositionVector(object, nullptr);
                float* direction = BuildFacingVectorBetween(
                    origin, nullptr, facingResult, position);
                std::memcpy(nearestDirection, direction, 0x0C);
                nearestDistance = distance;
            }
        }

        void* receiver = CSF_AT(
            void*, reinterpret_cast<void*>(GetGlobalState_50D190()), 0x74);
        if (receiver
            && !CSF_AT(uint8_t, reinterpret_cast<void*>(GetGlobalState_50D190()), 0x89))
        {
            if (nearestDistance != CSF_AT(float, self, 0x18))
            {
                if (static_cast<uint8_t>(GfxMatrix_AreVectorsDifferent(
                        nearestDirection, nullptr, &g_ZeroVectorX)))
                {
                    NormalizeVector3(
                        reinterpret_cast<C_00833b88*>(nearestDirection), nullptr);
                }

                std::memcpy(eventData, nearestDirection, 0x0C);
                eventData[3] = nearestDistance;
                ((void (__fastcall*)(void*, void*, int, float*))
                    CSF_AT(void**, receiver, 0)[0x04 / 4])(
                        receiver, nullptr, 5, eventData);
            }

            if (previousIntensity != *intensity
                && std::fabs(-*intensity) <= g_ProximityEpsilon)
            {
                ((void (__fastcall*)(void*, void*))
                    CSF_AT(void**, receiver, 0)[0x08 / 4])(receiver, nullptr);
            }
        }
    }

    void* output = CSF_AT(void*, self, 0x04);
    if (output)
        CSF_AT(float, output, 0x24) = *intensity;
}

HOOK(0x004F92A0, DevConsole_UpdateProximityIntensity, "auto");
