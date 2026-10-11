#include <cstdint>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, CharacterModel_FindViewEndpoints, 0x004F5FE0, (int a1, int edx0, int a3, float* a4, int a5, float* a6));
CSF_FUNC(int, __cdecl, RwV3dTransformVectors, 0x006230A0, (int a1, int a2, int a3, int a4));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(float*, __fastcall, NormalizeVector3, 0x006AB8D0, (C_00833b88* self, void*));
CSF_FUNC(int, __fastcall, GfxMatrix_CreateQuaternionMatrix, 0x006AE6F0, (float* self, void* , int outputAddress));
CSF_FUNC(float*, __fastcall, GfxMatrix_SetQuaternionFromAxisAngle, 0x006AE8C0, (float* self, void* , S_00526d40* axis, float angle));
CSF_FUNC(float*, __fastcall, GfxMatrix_SetQuaternionAxisAngle, 0x006AE970, (float* self, void* , S_00526d40* axis, float angle));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);
CSF_GLOBAL(float, g_NegativeDistanceTolerance, 0x008343FC);
CSF_GLOBAL(float, g_DegreesToRadians, 0x0083F184);
CSF_GLOBAL(float, g_PositiveDistanceTolerance, 0x0083F188);
CSF_GLOBAL(float, g_AxisY, 0x008A7D78);

static int __fastcall CharacterModel_UpdateViewTransform(
    void* self, void* /*edx*/, int character, int /*unused*/)
{
    alignas(8) std::uint8_t quaternionBytes[0x1C];
    alignas(8) std::uint8_t endpointBytes[0xC];
    alignas(8) std::uint8_t positionBytes[0xC];
    alignas(8) std::uint8_t originBytes[0xC];
    alignas(8) std::uint8_t matrixResultBytes[0x8];

    float* quaternion = reinterpret_cast<float*>(quaternionBytes);
    float* endpoint = reinterpret_cast<float*>(endpointBytes);
    float* position = reinterpret_cast<float*>(positionBytes);
    float* origin = reinterpret_cast<float*>(originBytes);
    float* direction = reinterpret_cast<float*>(
        reinterpret_cast<std::uint8_t*>(self) + 0xAC);

    void* owner = CSF_AT(void*, self, 0x4);
    void* view = reinterpret_cast<void* (__fastcall*)(void*, void*)>(
        CSF_AT(void**, owner, 0)[0x10 / 4])(owner, nullptr);
    float currentAngle = static_cast<float>(
        reinterpret_cast<double (__fastcall*)(void*, void*)>(
            CSF_AT(void**, view, 0)[0x24 / 4])(view, nullptr));

    int settings = GetGlobalValue_8BC1E0();
    if (currentAngle + CSF_AT(float, self, 0xB8) >
        CSF_AT(float, settings, 0x9C4))
    {
        settings = GetGlobalValue_8BC1E0();
        CSF_AT(float, self, 0xB8) =
            CSF_AT(float, settings, 0x9C4) - currentAngle;
    }
    else
    {
        settings = GetGlobalValue_8BC1E0();
        if (currentAngle + CSF_AT(float, self, 0xB8) <
            CSF_AT(float, settings, 0x9C0))
        {
            settings = GetGlobalValue_8BC1E0();
            CSF_AT(float, self, 0xB8) =
                CSF_AT(float, settings, 0x9C0) - currentAngle;
        }
    }

    GfxMatrix_SetQuaternionFromAxisAngle(
        quaternion, nullptr, reinterpret_cast<S_00526d40*>(&g_AxisY),
        CSF_AT(float, self, 0xBC) * g_DegreesToRadians);

    int matrixResult = GfxMatrix_CreateQuaternionMatrix(
        quaternion, nullptr, reinterpret_cast<int>(matrixResultBytes));
    int matrix = CSF_AT(int, matrixResult, 0);
    position[0] = 0.0f;
    position[1] = 0.0f;
    position[2] = 0.0f;
    RwV3dTransformVectors(
        reinterpret_cast<int>(position), reinterpret_cast<int>(direction),
        1, matrix);
    CSF_AT(float, self, 0xAC) = position[0];
    CSF_AT(float, self, 0xB0) = position[1];
    CSF_AT(float, self, 0xB4) = position[2];
    if (CSF_AT(std::uint8_t, matrixResultBytes, 0x4))
        RwMatrixDestroy(CSF_AT(std::uint32_t*, matrixResultBytes, 0));

    S_00526d40** axisResult =
        reinterpret_cast<S_00526d40** (__fastcall*)(void*, void*, void*)>(
            CSF_AT(void**, view, 0)[0x8C / 4])(
                view, nullptr, matrixResultBytes);
    GfxMatrix_SetQuaternionAxisAngle(
        quaternion, nullptr, *axisResult,
        CSF_AT(float, self, 0xB8) * g_DegreesToRadians);
    if (CSF_AT(std::uint8_t, matrixResultBytes, 0x4))
        RwMatrixDestroy(CSF_AT(std::uint32_t*, matrixResultBytes, 0));

    matrixResult = GfxMatrix_CreateQuaternionMatrix(
        quaternion, nullptr, reinterpret_cast<int>(matrixResultBytes));
    matrix = CSF_AT(int, matrixResult, 0);
    position[0] = 0.0f;
    position[1] = 0.0f;
    position[2] = 0.0f;
    RwV3dTransformVectors(
        reinterpret_cast<int>(position), reinterpret_cast<int>(direction),
        1, matrix);
    CSF_AT(float, self, 0xAC) = position[0];
    CSF_AT(float, self, 0xB0) = position[1];
    CSF_AT(float, self, 0xB4) = position[2];
    if (CSF_AT(std::uint8_t, matrixResultBytes, 0x4))
        RwMatrixDestroy(CSF_AT(std::uint32_t*, matrixResultBytes, 0));

    NormalizeVector3(reinterpret_cast<C_00833b88*>(direction), nullptr);
    endpoint[0] = 0.0f;
    endpoint[1] = 0.0f;
    endpoint[2] = 0.0f;
    origin[0] = 0.0f;
    origin[1] = 0.0f;
    origin[2] = 0.0f;
    CharacterModel_FindViewEndpoints(
        reinterpret_cast<int>(self), reinterpret_cast<int>(origin),
        reinterpret_cast<int>(endpoint), origin, character, direction);

    settings = GetGlobalValue_8BC1E0();
    float transition = CSF_AT(float, self, 0xC8) /
                       CSF_AT(float, settings, 0x9B8);
    if (transition > g_TransitionOne)
        transition = g_TransitionOne;
    float distance =
        (CSF_AT(float, self, 0xC4) - CSF_AT(float, self, 0xC0)) *
            transition +
        CSF_AT(float, self, 0xC0);

    float dx = endpoint[0] - origin[0];
    float dy = endpoint[1] - origin[1];
    float dz = endpoint[2] - origin[2];
    float targetDistance = std::sqrt(dz * dz + dy * dy + dx * dx);
    float difference = targetDistance - CSF_AT(float, self, 0xC4);
    if (difference < g_NegativeDistanceTolerance ||
        difference > g_PositiveDistanceTolerance)
    {
        CSF_AT(float, self, 0xC4) = targetDistance;
        CSF_AT(std::uint32_t, self, 0xC8) = 0;
        if (distance > targetDistance)
            distance = targetDistance;
        CSF_AT(float, self, 0xC0) = distance;
    }

    position[0] = distance * CSF_AT(float, self, 0xAC) + origin[0];
    position[1] = distance * CSF_AT(float, self, 0xB0) + origin[1];
    position[2] = distance * CSF_AT(float, self, 0xB4) + origin[2];
    reinterpret_cast<void (__fastcall*)(void*, void*, float*)>(
        CSF_AT(void**, view, 0)[0x14 / 4])(view, nullptr, position);
    reinterpret_cast<void (__fastcall*)(void*, void*, float*)>(
        CSF_AT(void**, view, 0)[0x54 / 4])(view, nullptr, origin);

    settings = GetGlobalValue_8BC1E0();
    float blend = distance / CSF_AT(float, settings, 0x9B4);
    if (blend >= g_TransitionOne)
        blend = g_TransitionOne;
    void* characterObject = reinterpret_cast<void*>(character);
    reinterpret_cast<void (__fastcall*)(void*, void*, float)>(
        CSF_AT(void**, characterObject, 0)[0x70 / 4])(
            characterObject, nullptr, blend);

    CSF_AT(std::uint32_t, self, 0xB8) = 0;
    CSF_AT(std::uint32_t, self, 0xBC) = 0;
    return 0;
}

HOOK(0x004F7400, CharacterModel_UpdateViewTransform, "auto");
