#include <cstdint>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_ConfigureFocusEntry, 0x0048E460, (void* self, void* edx, int* a2));
CSF_FUNC(double, __fastcall, ComputeScalarForResolvedObject, 0x0048F220, (void* self, void* edx));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_GLOBAL(float, g_DegreesToRadians, 0x0082DFC0);

static void __fastcall UpdateAttachedObjectTransform(void* self, void* /*edx*/)
{
    if (!CSF_AT(uint8_t, self, 0x115))
        return;

    void* anchor = reinterpret_cast<void*>(CHandleTable_Resolve(
        g_pCurrentScene, nullptr,
        reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0x118)));

    if (!anchor)
    {
        CSF_AT(uint8_t, self, 0x115) = 0;
        CSF_AT(int, self, 0x11C) = -1;
        CSF_AT(int, self, 0x120) = -1;
        return;
    }

    float delta[4];
    float* position = reinterpret_cast<float*>(GetObjectPositionVector(anchor, nullptr));
    delta[0] = position[0] - CSF_AT(float, self, 0x124);
    delta[1] = position[1] - CSF_AT(float, self, 0x128);
    delta[2] = position[2] - CSF_AT(float, self, 0x12C);
    delta[3] = static_cast<float>(
        ComputeScalarForResolvedObject(anchor, nullptr) - CSF_AT(float, self, 0x130));

    if (CSF_AT(uint8_t, self, 0x88))
    {
        position = reinterpret_cast<float*>(GetObjectPositionVector(
            CSF_AT(void*, self, 0x4), nullptr));

        float translated[3];
        translated[2] = delta[2] + position[2];
        translated[1] = delta[1] + position[1];
        translated[0] = delta[0] + position[0];
        SceneLoader_ConfigureFocusEntry(
            CSF_AT(void*, self, 0x4), nullptr, reinterpret_cast<int*>(translated));

        void* object = CSF_AT(void*, self, 0x4);
        void* component = reinterpret_cast<void*>(
            (reinterpret_cast<int(__fastcall*)(void*, void*)>(
                CSF_AT(void**, object, 0)[0x28 / 4]))(object, nullptr));
        void* rotation = reinterpret_cast<void*>(
            (reinterpret_cast<int(__fastcall*)(void*, void*)>(
                CSF_AT(void**, component, 0)[0x18 / 4]))(component, nullptr));
        (reinterpret_cast<void(__fastcall*)(void*, void*, float)>(
            CSF_AT(void**, rotation, 0)[0x40 / 4]))(rotation, nullptr, delta[3]);

        float* anchorPosition =
            reinterpret_cast<float*>(GetObjectPositionVector(anchor, nullptr));
        float* objectPosition = reinterpret_cast<float*>(GetObjectPositionVector(
            CSF_AT(void*, self, 0x4), nullptr));

        const float relativeX = objectPosition[0] - anchorPosition[0];
        const float relativeY = objectPosition[1] - anchorPosition[1];
        const float relativeZ = objectPosition[2] - anchorPosition[2];
        const float angle = delta[3] * g_DegreesToRadians;
        delta[3] = std::cos(angle);
        const float sine = std::sin(angle);

        translated[0] = relativeZ * sine + relativeX * delta[3];
        translated[2] = relativeZ * delta[3] - relativeX * sine;

        position = reinterpret_cast<float*>(GetObjectPositionVector(anchor, nullptr));
        delta[2] = translated[2] + position[2];
        delta[1] = relativeY + position[1];
        delta[0] = translated[0] + position[0];
        SceneLoader_ConfigureFocusEntry(
            CSF_AT(void*, self, 0x4), nullptr, reinterpret_cast<int*>(delta));
    }

    void* currentPosition =
        reinterpret_cast<void*>(GetObjectPositionVector(anchor, nullptr));
    CSF_AT(uint32_t, self, 0x124) = CSF_AT(uint32_t, currentPosition, 0x0);
    CSF_AT(uint32_t, self, 0x128) = CSF_AT(uint32_t, currentPosition, 0x4);
    CSF_AT(uint32_t, self, 0x12C) = CSF_AT(uint32_t, currentPosition, 0x8);
    CSF_AT(float, self, 0x130) =
        static_cast<float>(ComputeScalarForResolvedObject(anchor, nullptr));
}

HOOK(0x004133A0, UpdateAttachedObjectTransform, "auto");
