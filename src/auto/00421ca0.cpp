#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_0082dd78_DispatchReturnedObject, 0x0041E0C0, (C_0082dd78* self, void*));
CSF_FUNC(int, __fastcall, SceneObject_PassesFirstCheck, 0x0048E120, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __fastcall, IsField1BCNonNegative, 0x00492B10, (void* self, void*));
CSF_FUNC(uint32_t*, __fastcall, S_00526d40_ConvertDirection, 0x006ABA10, (S_00526d40* self, void* , uint32_t* output));
CSF_FUNC(float*, __fastcall, GfxMatrix_GetBoundingSphere, 0x006AC530, (float* self, void* , float* output));
CSF_FUNC(bool, __fastcall, GfxMatrix_ForwardValueWithAddress, 0x006ACA20, (void* self, void* , int a2));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(float, g_WeaponPlacementTraceDepth, 0x0082E11C);
CSF_GLOBAL(float, g_QuarterTurnConstant, 0x0082FAD4);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_GLOBAL(float, g_ZeroVectorX, 0x008BB420);

static uint32_t* __fastcall AiTaskStates_ApplyNearbyImpulse(void* self, void* /*edx*/)
{
    float* initialPosition = reinterpret_cast<float*>(
        GetObjectPositionVector(CSF_AT(C_008342e8*, self, 0x14), nullptr));

    void* scene = g_pCurrentScene;
    const uint32_t count = CSF_AT(uint32_t, scene, 0x68);
    void* objectList = static_cast<uint8_t*>(scene) + 0x60;

    if (count == 0)
        return static_cast<uint32_t*>(objectList);

    for (uint32_t index = 0; index < count; ++index)
    {
        C_008342e8* candidate = CSF_AT(C_008342e8**, objectList, 0)[index];
        C_0082dd78* controller =
            reinterpret_cast<C_0082dd78*>(candidate->field_004c);

        float* candidatePosition = reinterpret_cast<float*>(
            GetObjectPositionVector(candidate, nullptr));
        const float dx = candidatePosition[0] - initialPosition[0];
        const float dy = candidatePosition[1] - initialPosition[1];
        const float dz = candidatePosition[2] - initialPosition[2];

        if (!(dx * dx + dy * dy + dz * dz < g_WeaponPlacementTraceDepth))
            continue;
        if (static_cast<uint8_t>(IsField1BCNonNegative(candidate, nullptr)) != 0)
            continue;
        if (static_cast<uint8_t>(SceneObject_PassesFirstCheck(candidate, nullptr)) != 0)
            continue;
        if (((int(__fastcall*)(C_0082dd78*, void*))controller->vtbl[0x4C / 4])(
                controller, nullptr) != 0)
            continue;

        float sourceBounds[0x3C / sizeof(float)];
        float candidateBounds[0x3C / sizeof(float)];

        C_008342e8* owner = CSF_AT(C_008342e8*, self, 0x14);
        void* sourceObject = reinterpret_cast<void*>(
            C_0082dd78_DispatchReturnedObject(
                reinterpret_cast<C_0082dd78*>(owner->field_004c), nullptr));
        void* sourceGeometry =
            ((void*(__fastcall*)(void*, void*))CSF_AT(void**, sourceObject, 0)[0x34 / 4])(
                sourceObject, nullptr);
        ((void(__fastcall*)(void*, void*, void*))CSF_AT(void**, sourceGeometry, 0)[0x1C / 4])(
            sourceGeometry, nullptr, sourceBounds);

        void* candidateObject = reinterpret_cast<void*>(
            C_0082dd78_DispatchReturnedObject(
                reinterpret_cast<C_0082dd78*>(candidate->field_004c), nullptr));
        void* candidateGeometry =
            ((void*(__fastcall*)(void*, void*))CSF_AT(void**, candidateObject, 0)[0x34 / 4])(
                candidateObject, nullptr);
        ((void(__fastcall*)(void*, void*, void*))CSF_AT(void**, candidateGeometry, 0)[0x1C / 4])(
            candidateGeometry, nullptr, candidateBounds);

        float sphere[0x24 / sizeof(float)]{};
        GfxMatrix_GetBoundingSphere(candidateBounds, nullptr, sphere);
        sphere[3] *= g_HalfDurationScale;

        if (!GfxMatrix_ForwardValueWithAddress(
                sourceBounds, nullptr, reinterpret_cast<int>(sphere)))
            continue;

        float* ownerPosition = reinterpret_cast<float*>(
            GetObjectPositionVector(CSF_AT(C_008342e8*, self, 0x14), nullptr));
        candidatePosition = reinterpret_cast<float*>(
            GetObjectPositionVector(candidate, nullptr));

        float direction[3];
        direction[2] = candidatePosition[2] - ownerPosition[2];
        direction[1] = candidatePosition[1] - ownerPosition[1];
        direction[0] = candidatePosition[0] - ownerPosition[0];

        const float magnitude =
            CSF_AT(float, reinterpret_cast<void*>(GetGlobalValue_8BC1E0()), 0x938)
            * g_QuarterTurnConstant;

        uint32_t normalizedStorage[3];
        float* normalized = reinterpret_cast<float*>(
            S_00526d40_ConvertDirection(
                reinterpret_cast<S_00526d40*>(direction), nullptr, normalizedStorage));

        float impulse[4];
        impulse[2] = magnitude * normalized[2];
        impulse[1] = magnitude * normalized[1];
        impulse[0] = magnitude * normalized[0];
        std::memcpy(&impulse[3], &initialPosition, sizeof(initialPosition));

        ((void(__fastcall*)(C_0082dd78*, void*, float*, float*, uint32_t))
            controller->vtbl[0x54 / 4])(
                controller, nullptr, impulse, &g_ZeroVectorX, 0);
    }

    return reinterpret_cast<uint32_t*>(count);
}

HOOK(0x00421CA0, AiTaskStates_ApplyNearbyImpulse, "auto");
