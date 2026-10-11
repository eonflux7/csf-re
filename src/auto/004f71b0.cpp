#include <cstdint>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, S_00526d40_TransformVector3, 0x004069F0, (S_00526d40* self, void* , uint32_t* out, int* in));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(void, __fastcall, CharacterModel_ComputePositioningEndpoints, 0x004F5FE0, (int a1, int edx0, int a3, float* a4, int a5, float* a6));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(float*, __fastcall, NormalizeVector3, 0x006AB8D0, (C_00833b88* self, void*));
CSF_FUNC(float*, __fastcall, GfxMatrix_EulerAnglesToDirection, 0x006AE640, (float* self, void* , float* out));
CSF_FUNC(int, __fastcall, GfxMatrix_CreateQuaternionMatrix, 0x006AE6F0, (float* self, void* , int outputAddress));
CSF_FUNC(float*, __fastcall, GfxMatrix_SetQuaternionFromAxisAngle, 0x006AE8C0, (float* self, void* , S_00526d40* axis, float angle));
CSF_FUNC(int, __cdecl, GetGlobalValue_753A30, 0x00753A30, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(float, g_MissingMetricValue, 0x0082D4C4);
CSF_GLOBAL(float, g_DegreesToRadians, 0x0083F184);
CSF_GLOBAL(float, g_PositioningAngleLimit, 0x0083F19C);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __fastcall CharacterModel_CommitPositioning(
    void* self, void* /*edx*/, float directionFlag)
{
    if (CSF_AT(int, self, 0x8) == 8)
        return 8;

    CSF_AT(int, self, 0x8) = 8;
    const int resolved = CHandleTable_Resolve(
        g_pCurrentScene, nullptr,
        reinterpret_cast<int>(static_cast<std::uint8_t*>(self) + 0x74));
    if (!resolved)
        return resolved;

    void* object = reinterpret_cast<void*>(resolved);
    const int objectState = CSF_AT(int, object, 0x50);
    if (objectState)
        return objectState;

    alignas(8) std::uint8_t quaternionBytes[0x10];
    alignas(8) std::uint8_t vectorBytes[0xC];
    alignas(8) std::uint8_t startBytes[0xC];
    alignas(8) std::uint8_t endBytes[0xC];
    alignas(8) std::uint8_t matrixBytes[0xC];

    float* quaternion = reinterpret_cast<float*>(quaternionBytes);
    auto* vectorOutput = reinterpret_cast<std::uint32_t*>(vectorBytes);
    float* start = reinterpret_cast<float*>(startBytes);
    float* end = reinterpret_cast<float*>(endBytes);
    auto* directionBytes = static_cast<std::uint8_t*>(self) + 0xAC;
    float* direction = reinterpret_cast<float*>(directionBytes);

    // The vector output argument is for EulerAnglesToDirection, not the
    // preceding virtual getter.
    float* eulerAngles =
        reinterpret_cast<float* (__fastcall*)(void*, void*, void*)>(
            CSF_AT(void**, object, 0)[0xAC / 4])(
                object, nullptr, matrixBytes);
    float* initialDirection = GfxMatrix_EulerAnglesToDirection(
        eulerAngles, nullptr, reinterpret_cast<float*>(vectorBytes));

    CSF_AT(std::uint32_t, self, 0xAC) =
        CSF_AT(std::uint32_t, initialDirection, 0);
    CSF_AT(std::uint32_t, self, 0xB0) =
        CSF_AT(std::uint32_t, initialDirection, 4);
    CSF_AT(std::uint32_t, self, 0xB4) =
        CSF_AT(std::uint32_t, initialDirection, 8);
    CSF_AT(std::uint32_t, self, 0xB0) = 0;

    const bool forward =
        reinterpret_cast<const std::uint8_t*>(&directionFlag)[0] != 0;
    if (!forward)
    {
        CSF_AT(float, self, 0xAC) *= g_MissingMetricValue;
        CSF_AT(float, self, 0xB0) *= g_MissingMetricValue;
        CSF_AT(float, self, 0xB4) *= g_MissingMetricValue;
    }

    NormalizeVector3(reinterpret_cast<C_00833b88*>(direction), nullptr);
    CSF_AT(std::uint32_t, self, 0xB8) = 0;
    CSF_AT(std::uint32_t, self, 0xBC) = 0;

    CSF_AT(float, matrixBytes, 8) =
        CSF_AT(float, reinterpret_cast<void*>(GetGlobalValue_8BC1E0()), 0x9BC)
        * g_DegreesToRadians;
    if (forward)
        CSF_AT(float, matrixBytes, 8) = -CSF_AT(float, matrixBytes, 8);

    void* controller = CSF_AT(void*, self, 4);
    void* model =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, controller, 0)[0x10 / 4])(
                controller, nullptr);
    auto** axis =
        reinterpret_cast<S_00526d40** (__fastcall*)(void*, void*, void*)>(
            CSF_AT(void**, model, 0)[0x8C / 4])(
                model, nullptr, matrixBytes);
    GfxMatrix_SetQuaternionFromAxisAngle(
        quaternion, nullptr, *axis, CSF_AT(float, matrixBytes, 8));

    if (CSF_AT(std::uint8_t, matrixBytes, 4))
        RwMatrixDestroy(CSF_AT(std::uint32_t*, matrixBytes, 0));

    const int rotationMatrix = GfxMatrix_CreateQuaternionMatrix(
        quaternion, nullptr, reinterpret_cast<int>(matrixBytes));
    std::uint32_t* transformed = S_00526d40_TransformVector3(
        reinterpret_cast<S_00526d40*>(direction), nullptr,
        vectorOutput, reinterpret_cast<int*>(rotationMatrix));

    CSF_AT(std::uint32_t, self, 0xAC) = transformed[0];
    CSF_AT(std::uint32_t, self, 0xB0) = transformed[1];
    CSF_AT(std::uint32_t, self, 0xB4) = transformed[2];

    if (CSF_AT(std::uint8_t, matrixBytes, 4))
        RwMatrixDestroy(CSF_AT(std::uint32_t*, matrixBytes, 0));

    NormalizeVector3(reinterpret_cast<C_00833b88*>(direction), nullptr);

    end[0] = 0.0f;
    end[1] = 0.0f;
    end[2] = 0.0f;
    start[0] = 0.0f;
    start[1] = 0.0f;
    start[2] = 0.0f;
    CharacterModel_ComputePositioningEndpoints(
        reinterpret_cast<int>(self), reinterpret_cast<int>(start),
        reinterpret_cast<int>(end), start, resolved, direction);

    const float dx = end[0] - start[0];
    const float dy = end[1] - start[1];
    CSF_AT(std::uint32_t, self, 0xC8) = 0;
    CSF_AT(std::uint32_t, self, 0xC0) = 0;
    const float dz = end[2] - start[2];
    CSF_AT(float, self, 0xC4) = std::sqrt(dz * dz + dy * dy + dx * dx);

    float baseAngle =
        CSF_AT(float, reinterpret_cast<void*>(GetGlobalValue_753A30()), 0x13C)
        * g_DegreesToRadians;
    void* settings = reinterpret_cast<void*>(GetValue8B0554());
    if (CSF_AT(int, settings, 0x90C) != -1082130432)
    {
        baseAngle =
            CSF_AT(float, static_cast<std::uint8_t*>(settings) + 0x54, 0x8B8)
            * g_DegreesToRadians;
    }

    float angle =
        CSF_AT(float, reinterpret_cast<void*>(GetGlobalValue_8BC1E0()), 0x9C8)
        * g_DegreesToRadians + baseAngle;
    if (angle > g_PositioningAngleLimit)
        angle = 2.3561945f;

    controller = CSF_AT(void*, self, 4);
    return reinterpret_cast<int (__fastcall*)(void*, void*, float)>(
        CSF_AT(void**, controller, 0)[0x20 / 4])(
            controller, nullptr, angle);
}

HOOK(0x004F71B0, CharacterModel_CommitPositioning, "auto");
