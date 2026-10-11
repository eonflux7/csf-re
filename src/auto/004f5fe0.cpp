#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint8_t*, __fastcall, Container_FindFirstMatchingEnabledEntry, 0x004505A0, (void* self, void* , int a2));
CSF_FUNC(float*, __fastcall, CharacterModel_GetPosition, 0x00494C30, (void* self, void* edx, float* a2));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __stdcall, CharacterModel_AdjustCameraFromHit, 0x004F5780, (float* a1, float* a2, float* a3, float* a4, float* a5));
CSF_FUNC(int, __cdecl, RwV3dTransformVectors, 0x006230A0, (int a1, int a2, int a3, int a4));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(int, __fastcall, SceneLoader_GetSceneController, 0x00652010, (void* self, void* edx));
CSF_FUNC(float*, __fastcall, NormalizeVector3, 0x006AB8D0, (C_00833b88* self, void*));
CSF_FUNC(double, __fastcall, GfxPlane_RayIntersectionT, 0x006AD8C0, (float* self, void* , float* rayOrigin, float* rayDirection));
CSF_FUNC(int, __fastcall, GfxMatrix_OrientAlongVector, 0x006ADFA0, (S_006ada10* self, void* , void* sourceData));
CSF_FUNC(int, __fastcall, GfxMatrix_CreateQuaternionMatrix, 0x006AE6F0, (float* self, void* , int outputAddress));
CSF_FUNC(float*, __fastcall, GfxMatrix_SetQuaternionFromAxisAngle, 0x006AE8C0, (float* self, void* , S_00526d40* axis, float angle));
CSF_FUNC(float*, __fastcall, GfxMatrix_SetQuaternionAxisAngle, 0x006AE970, (float* self, void* , S_00526d40* axis, float angle));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_fHudFadeStep, 0x0082E04C);
CSF_GLOBAL(float, g_fIntroVideoDuration, 0x0082E118);
CSF_GLOBAL(float, g_CameraHeightOffset, 0x0083F190);
CSF_GLOBAL(float, g_AxisY, 0x008A7D78);
CSF_GLOBAL(float, g_AxisY_Y, 0x008A7D7C);
CSF_GLOBAL(float, g_AxisY_Z, 0x008A7D80);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static void CharacterModel_CopyPoint(float* destination, const float* source)
{
    destination[0] = source[0];
    destination[1] = source[1];
    destination[2] = source[2];
}

static float CharacterModel_PointDistance(const float* first, const float* second)
{
    const float x = first[0] - second[0];
    const float y = first[1] - second[1];
    const float z = first[2] - second[2];
    return std::sqrt(x * x + z * z + y * y);
}

static void CharacterModel_InitializeCollisionResult(uint8_t* result)
{
    CSF_AT(float, result, 0x04) = 0.0f;
    CSF_AT(float, result, 0x08) = 0.0f;
    CSF_AT(float, result, 0x0C) = 0.0f;
    CSF_AT(int, result, 0x10) = 0;
    CSF_AT(int, result, 0x14) = 0;
    CSF_AT(int, result, 0x18) = 0;
    CSF_AT(int, result, 0x5C) = 0;
    CSF_AT(uint8_t, result, 0x28) = 0;
    CSF_AT(int, result, 0x60) = 0;
    CSF_AT(int, result, 0x24) = 0;
    CSF_AT(int, result, 0x2C) = -1;
    CSF_AT(int, result, 0x1C) = -1;
    CSF_AT(uint8_t, result, 0x30) = 0xFF;
    CSF_AT(int, result, 0x64) = 10;
    CSF_AT(int, result, 0x00) = 2139095039;
    CSF_AT(uint8_t, result, 0x20) = 0xFF;
}

// Resolve the camera endpoint, then back it away from nearby geometry.
static char __fastcall CharacterModel_ResolveCameraPosition(
    void* self, void* /*edx*/, float* cameraPosition, float* origin,
    int characterAddress, float* direction)
{
    void* character = reinterpret_cast<void*>(characterAddress);

    void* sceneController = reinterpret_cast<void*>(
        SceneLoader_GetSceneController(
            reinterpret_cast<void*>(GetGlobalInt()), nullptr));
    void* collisionScene =
        ((void*(__fastcall*)(void*, void*))
            CSF_AT(void**, sceneController, 0)[0x3C / 4])(
                sceneController, nullptr);

    alignas(8) uint8_t positionBytes[0x0C];
    float* position = CharacterModel_GetPosition(
        character, nullptr, reinterpret_cast<float*>(positionBytes));
    CharacterModel_CopyPoint(origin, position);

    if ((CSF_AT(uint32_t, character, 0x2E0) & 0x10000) != 0 &&
        CSF_AT(uint8_t,
            CSF_AT(void*, reinterpret_cast<void*>(GetValue8B0554()), 0x1FDC),
            0x2C) != 0 &&
        ((CSF_AT(uint32_t, character, 0x2E0) & 0x10000) == 0 ||
         CSF_AT(uint8_t, character, 0x3D0) == 0))
    {
        origin[1] = CSF_AT(float,
            CSF_AT(void*, reinterpret_cast<void*>(GetValue8B0554()), 0x1FDC),
            0x30) + g_CameraHeightOffset;
    }

    alignas(8) uint8_t temporaryBytes[0x0C];
    float* temporary = reinterpret_cast<float*>(temporaryBytes);
    const float cameraDistance = CSF_AT(
        float, reinterpret_cast<void*>(GetGlobalValue_8BC1E0()), 0x9B0);
    temporary[0] = cameraDistance * direction[0] + origin[0];
    temporary[1] = cameraDistance * direction[1] + origin[1];
    temporary[2] = cameraDistance * direction[2] + origin[2];
    CharacterModel_CopyPoint(cameraPosition, temporary);

    alignas(8) uint8_t reverseBytes[0x10];
    float* reverse = reinterpret_cast<float*>(reverseBytes);
    alignas(8) uint8_t rayBytes[0x18];
    float* ray = reinterpret_cast<float*>(rayBytes);
    reverse[0] = -direction[0];
    ray[0] = direction[0];
    ray[1] = direction[1];
    reverse[1] = -direction[1];
    ray[2] = direction[2];
    reverse[2] = -direction[2];
    CharacterModel_CopyPoint(ray + 3, origin);
    NormalizeVector3(reinterpret_cast<C_00833b88*>(rayBytes), nullptr);

    if (CSF_AT(uint8_t,
        CSF_AT(void*, reinterpret_cast<void*>(GetValue8B0554()), 0x1FDC),
        0x2C) != 0)
    {
        const float height = CSF_AT(float,
            CSF_AT(void*, reinterpret_cast<void*>(GetValue8B0554()), 0x1FDC),
            0x30);

        alignas(8) uint8_t planeBytes[0x2C];
        float* plane = reinterpret_cast<float*>(planeBytes);
        if (!(height < origin[1]))
        {
            plane[3] = -g_AxisY;
            plane[4] = -g_AxisY_Y;
            plane[5] = -g_AxisY_Z;
        }
        else
        {
            plane[3] = g_AxisY;
            plane[4] = g_AxisY_Y;
            plane[5] = g_AxisY_Z;
        }
        plane[0] = 0.0f;
        plane[1] = plane[4] * g_fHudFadeStep + height;
        plane[2] = 0.0f;
        CSF_AT(uint8_t, planeBytes, 0x28) = 1;

        const double intersection =
            GfxPlane_RayIntersectionT(plane, nullptr, origin, direction);
        const float intersectionDistance = static_cast<float>(intersection);
        if (intersection > g_fZero &&
            CharacterModel_PointDistance(origin, cameraPosition) >
                intersectionDistance)
        {
            temporary[0] = intersectionDistance * direction[0] + origin[0];
            temporary[1] = intersectionDistance * direction[1] + origin[1];
            temporary[2] = intersectionDistance * direction[2] + origin[2];
            CharacterModel_CopyPoint(cameraPosition, temporary);
        }
    }

    // The six endpoint floats and seven direction vectors were contiguous locals.
    alignas(8) uint8_t segmentAndDirectionsBytes[0x6C];
    float* segment = reinterpret_cast<float*>(segmentAndDirectionsBytes);
    float* probeDirections = segment + 6;
    CharacterModel_CopyPoint(segment, origin);
    CharacterModel_CopyPoint(segment + 3, cameraPosition);

    alignas(8) uint8_t queryFlagsBytes[0x04];
    int* queryFlags = reinterpret_cast<int*>(queryFlagsBytes);
    *queryFlags = 32;
    alignas(8) uint8_t collisionResultBytes[0x68];
    float* hitPosition = reinterpret_cast<float*>(collisionResultBytes + 4);

    void* hit =
        ((void*(__fastcall*)(void*, void*, int, float*, int*, int))
            CSF_AT(void**, collisionScene, 0)[0x34 / 4])(
                collisionScene, nullptr, 0, segment, queryFlags, 3);
    if (hit)
    {
        CharacterModel_InitializeCollisionResult(collisionResultBytes);
        ((void(__fastcall*)(void*, void*, int*))
            CSF_AT(void**, hit, 0)[0x0C / 4])(
                hit, nullptr, reinterpret_cast<int*>(collisionResultBytes));
        CharacterModel_AdjustCameraFromHit(
            cameraPosition, hitPosition, origin, reverse, direction);
    }

    uint8_t* sceneEntries =
        reinterpret_cast<uint8_t*>(g_pCurrentScene) + 0x88;
    const uint32_t entryCount = CSF_AT(uint32_t, g_pCurrentScene, 0x90);
    for (uint32_t index = 0; index < entryCount; ++index)
    {
        void* entry = CSF_AT(void**, sceneEntries, 0)[index];
        if (entry)
        {
            temporary[0] = 0.0f;
            temporary[1] = 0.0f;
            temporary[2] = 0.0f;
            if (((bool(__fastcall*)(void*, void*, float*, float*))
                    CSF_AT(void**, entry, 0)[0x88 / 4])(
                        entry, nullptr, temporary, ray))
            {
                if (CharacterModel_PointDistance(origin, cameraPosition) >
                    CharacterModel_PointDistance(origin, temporary))
                {
                    CharacterModel_AdjustCameraFromHit(
                        cameraPosition, temporary, origin, reverse, direction);
                }
            }
        }
    }

    void* enabledEntry = Container_FindFirstMatchingEnabledEntry(
        self, nullptr, reinterpret_cast<int>(cameraPosition));
    if (enabledEntry)
    {
        temporary[0] = 0.0f;
        temporary[1] = 0.0f;
        temporary[2] = 0.0f;
        if (((bool(__fastcall*)(void*, void*, float*, float*))
                CSF_AT(void**, enabledEntry, 0)[0x88 / 4])(
                    enabledEntry, nullptr, temporary, ray))
        {
            CharacterModel_AdjustCameraFromHit(
                cameraPosition, temporary, origin, reverse, direction);
        }
    }

    bool reachedOrigin = false;
    alignas(8) uint8_t matrixBytes[0x0C];
    S_006ada10& matrix = *reinterpret_cast<S_006ada10*>(matrixBytes);
    matrix.field_0000 = reinterpret_cast<uint32_t>(RwMatrixCreate());
    CSF_AT(uint8_t, matrixBytes, 0x04) = 1;
    GfxMatrix_OrientAlongVector(&matrix, nullptr, reverse);

    std::memset(probeDirections, 0, 0x54);
    uint8_t* orientation = reinterpret_cast<uint8_t*>(matrix.field_0000);
    float* forward = reinterpret_cast<float*>(orientation + 0x20);

    probeDirections[0] = CSF_AT(float, orientation, 0x00);
    probeDirections[1] = CSF_AT(float, orientation, 0x04);
    probeDirections[2] = CSF_AT(float, orientation, 0x08);
    probeDirections[3] = -probeDirections[0];
    probeDirections[4] = -probeDirections[1];
    probeDirections[5] = -probeDirections[2];

    probeDirections[6] = CSF_AT(float, orientation, 0x10);
    probeDirections[7] = CSF_AT(float, orientation, 0x14);
    probeDirections[8] = CSF_AT(float, orientation, 0x18);
    probeDirections[9] = -probeDirections[6];
    probeDirections[10] = -probeDirections[7];
    probeDirections[11] = -probeDirections[8];

    probeDirections[12] = -CSF_AT(float, orientation, 0x20);
    probeDirections[13] = -CSF_AT(float, orientation, 0x24);
    probeDirections[14] = -CSF_AT(float, orientation, 0x28);

    alignas(8) uint8_t quaternionBytes[0x10];
    float* quaternion = reinterpret_cast<float*>(quaternionBytes);
    alignas(8) uint8_t rotationBytes[0x08];
    GfxMatrix_SetQuaternionFromAxisAngle(
        quaternion, nullptr,
        reinterpret_cast<S_00526d40*>(probeDirections + 6), 0.78539819f);
    int rotationResult = GfxMatrix_CreateQuaternionMatrix(
        quaternion, nullptr, reinterpret_cast<int>(rotationBytes));
    std::uint32_t* rotationMatrix = CSF_AT(
        std::uint32_t*, reinterpret_cast<void*>(rotationResult), 0);
    temporary[0] = 0.0f;
    temporary[1] = 0.0f;
    temporary[2] = 0.0f;
    RwV3dTransformVectors(
        reinterpret_cast<int>(temporary), reinterpret_cast<int>(forward), 1,
        reinterpret_cast<int>(rotationMatrix));
    CharacterModel_CopyPoint(probeDirections + 15, temporary);
    if (CSF_AT(uint8_t, rotationBytes, 0x04))
        RwMatrixDestroy(CSF_AT(std::uint32_t*, rotationBytes, 0));

    alignas(8) uint8_t axisBytes[0x0C];
    float* axis = reinterpret_cast<float*>(axisBytes);
    axis[0] = -probeDirections[6];
    axis[1] = -probeDirections[7];
    axis[2] = -probeDirections[8];
    GfxMatrix_SetQuaternionAxisAngle(
        quaternion, nullptr, reinterpret_cast<S_00526d40*>(axisBytes),
        0.78539819f);
    rotationResult = GfxMatrix_CreateQuaternionMatrix(
        quaternion, nullptr, reinterpret_cast<int>(rotationBytes));
    rotationMatrix = CSF_AT(
        std::uint32_t*, reinterpret_cast<void*>(rotationResult), 0);
    temporary[0] = 0.0f;
    temporary[1] = 0.0f;
    temporary[2] = 0.0f;
    RwV3dTransformVectors(
        reinterpret_cast<int>(temporary), reinterpret_cast<int>(forward), 1,
        reinterpret_cast<int>(rotationMatrix));
    CharacterModel_CopyPoint(probeDirections + 18, temporary);
    if (CSF_AT(uint8_t, rotationBytes, 0x04))
        RwMatrixDestroy(CSF_AT(std::uint32_t*, rotationBytes, 0));

    bool foundObstruction;
    do
    {
        foundObstruction = false;
        for (uint32_t index = 0; index < 7 && !foundObstruction; ++index)
        {
            float* probe = probeDirections + index * 3;
            CharacterModel_CopyPoint(segment, cameraPosition);
            temporary[0] = probe[0] * g_fHudFadeStep + cameraPosition[0];
            temporary[1] = probe[1] * g_fHudFadeStep + cameraPosition[1];
            temporary[2] = probe[2] * g_fHudFadeStep + cameraPosition[2];
            CharacterModel_CopyPoint(segment + 3, temporary);

            hit =
                ((void*(__fastcall*)(void*, void*, int, float*, int*, int))
                    CSF_AT(void**, collisionScene, 0)[0x34 / 4])(
                        collisionScene, nullptr, 0, segment, queryFlags, 3);
            if (hit)
            {
                CharacterModel_InitializeCollisionResult(collisionResultBytes);
                ((void(__fastcall*)(void*, void*, int*))
                    CSF_AT(void**, hit, 0)[0x0C / 4])(
                        hit, nullptr,
                        reinterpret_cast<int*>(collisionResultBytes));

                float retreat = g_fHudFadeStep -
                    CharacterModel_PointDistance(cameraPosition, hitPosition);
                if (retreat < g_fIntroVideoDuration)
                    retreat = g_fIntroVideoDuration;

                if (CharacterModel_PointDistance(origin, cameraPosition) -
                        retreat >= g_fHudFadeStep)
                {
                    const float zOffset = reverse[2] * retreat;
                    const float yOffset = reverse[1] * retreat;
                    cameraPosition[0] =
                        reverse[0] * retreat + cameraPosition[0];
                    cameraPosition[1] = yOffset + cameraPosition[1];
                    cameraPosition[2] = zOffset + cameraPosition[2];
                }
                else
                {
                    reachedOrigin = true;
                    temporary[0] = origin[0] + direction[0];
                    temporary[1] = origin[1] + direction[1];
                    temporary[2] = origin[2] + direction[2];
                    CharacterModel_CopyPoint(cameraPosition, temporary);
                }
                foundObstruction = true;
            }
        }
    } while (foundObstruction && !reachedOrigin);

    const char ownsMatrix = CSF_AT(char, matrixBytes, 0x04);
    if (ownsMatrix)
        return static_cast<char>(RwMatrixDestroy(
            reinterpret_cast<std::uint32_t*>(matrix.field_0000)));
    return ownsMatrix;
}

HOOK(0x004F5FE0, CharacterModel_ResolveCameraPosition, "auto");
