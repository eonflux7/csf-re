#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, InitS00402350Object, 0x00402350, (void* self, void* edx));
CSF_FUNC(int, __cdecl, RwV3dTransformPoints, 0x00623040, (int a1, int a2, int a3, int a4));
CSF_FUNC(std::uint32_t*, __cdecl, RwFrameGetLTM, 0x00623720, (std::uint32_t frame));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(int, __fastcall, Bounds_GetExtents, 0x006AB4A0, (void* self, void* edx, uint32_t* a2, uint32_t* a3, uint32_t* a4, uint32_t* a5, uint32_t* a6, uint32_t* a7));
CSF_FUNC(float*, __fastcall, SceneMissionSetup_GetBoundsCenter, 0x006AB660, (void* self, void* edx, float* a2));
CSF_FUNC(void, __fastcall, S_006abb80_ComputeAxisAlignedBounds, 0x006AC000, (S_006abb80* self, void* , float* maxX, float* minX, float* maxY, float* minY, float* maxZ, float* minZ));
CSF_FUNC(float*, __fastcall, Matrix_InvertWithStoredTarget, 0x006ADB40, (S_006adb40* self, void* , float** a2));
CSF_FUNC(bool, __fastcall, ObjectAttachment_CopyBounds, 0x006B80B0, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __fastcall, C_6B81C0_GetFirstItem, 0x006B81C0, (void* self, void*));
CSF_FUNC(int, __cdecl, SceneLoader_GetObjectAttachment, 0x006B87A0, (int a1));
CSF_FUNC(int, __cdecl, ObjectProvider_GetSceneObject, 0x006C0500, (int a1));
CSF_FUNC(uint32_t*, __cdecl, SceneObject_GetWorldBounds, 0x006C0570, (uint32_t* a1, int arg4, int a7));

CSF_GLOBAL(float, g_BoundsInterpolationFraction, 0x0082D820);
CSF_GLOBAL(float, g_BoundsPointHeightOffset, 0x0082D824);

static const auto kBoundsObjectReference = CSF_VTBL(0x00402350);
static const auto kMatrixObjectReference = CSF_VTBL(0x006ADB40);

static char __fastcall C_0082d844_InitializeLocalReferencePoints(
    C_0082d844* self, void* /*edx*/)
{
    void* owner = reinterpret_cast<void*>(self->field_0008);
    void* object =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, owner, 0)[0x28 / 4])(owner, nullptr);
    if (!object)
        return 0;

    void* provider = GetCachedObjectProvider(object, nullptr);
    const int sceneObject =
        ObjectProvider_GetSceneObject(reinterpret_cast<int>(provider));

    uint32_t boundsObject[0x3C / 4]{};
    uint32_t centerAndSceneObject[0x10 / 4]{};
    centerAndSceneObject[3] = static_cast<uint32_t>(sceneObject);

    InitS00402350Object(boundsObject, nullptr);
    void* attachment =
        reinterpret_cast<void*>(SceneLoader_GetObjectAttachment(sceneObject));

    float maxX = 0.0f;
    float minX = 0.0f;
    float maxY = 0.0f;
    float minY = 0.0f;
    float maxZ = 0.0f;
    float minZ = 0.0f;
    uint32_t worldBounds[0xC / 4]{};

    if (ObjectAttachment_CopyBounds(attachment, nullptr, boundsObject))
    {
        S_006abb80_ComputeAxisAlignedBounds(
            reinterpret_cast<S_006abb80*>(boundsObject), nullptr,
            &maxX, &minX, &maxY, &minY, &maxZ, &minZ);
    }
    else
    {
        C_6B81C0_GetFirstItem(attachment, nullptr);
        SceneObject_GetWorldBounds(worldBounds, sceneObject, 1);
        Bounds_GetExtents(
            worldBounds, nullptr,
            reinterpret_cast<uint32_t*>(&maxX),
            reinterpret_cast<uint32_t*>(&minX),
            reinterpret_cast<uint32_t*>(&maxY),
            reinterpret_cast<uint32_t*>(&minY),
            reinterpret_cast<uint32_t*>(&maxZ),
            reinterpret_cast<uint32_t*>(&minZ));
        SceneMissionSetup_GetBoundsCenter(
            worldBounds, nullptr,
            reinterpret_cast<float*>(centerAndSceneObject));
    }

    CSF_AT(float, self, 0x28) = 0.0f;
    CSF_AT(float, self, 0x2C) = 0.0f;
    CSF_AT(float, self, 0x30) = 0.0f;
    CSF_AT(float, self, 0x34) = 0.0f;
    CSF_AT(float, self, 0x38) = 0.0f;
    CSF_AT(float, self, 0x3C) = 0.0f;
    CSF_AT(float, self, 0x34) = maxX;

    // Both matrix references include an ownership byte at offset 4.
    uint32_t inverseMatrixStorage[0x8 / 4]{};
    CSF_AT(float*, inverseMatrixStorage, 0) =
        reinterpret_cast<float*>(RwMatrixCreate());
    CSF_AT(uint8_t, inverseMatrixStorage, 4) = 1;

    S_006adb40 frameMatrix;
    frameMatrix.field_0000 = reinterpret_cast<uint32_t>(
        RwFrameGetLTM(CSF_AT(
            uint32_t,
            reinterpret_cast<void*>(centerAndSceneObject[3]), 4)));
    frameMatrix.pad_0004[0] = 0;

    Matrix_InvertWithStoredTarget(
        &frameMatrix, nullptr,
        &CSF_AT(float*, inverseMatrixStorage, 0));

    float transformedPoint[3]{};
    RwV3dTransformPoints(
        reinterpret_cast<int>(transformedPoint),
        reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0x28),
        1, static_cast<int>(frameMatrix.field_0000));
    CSF_AT(float, self, 0x28) = transformedPoint[0];
    CSF_AT(float, self, 0x2C) = transformedPoint[1];
    CSF_AT(float, self, 0x30) = transformedPoint[2];

    RwV3dTransformPoints(
        reinterpret_cast<int>(transformedPoint),
        reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0x34),
        1, static_cast<int>(frameMatrix.field_0000));
    CSF_AT(float, self, 0x34) = transformedPoint[0];
    CSF_AT(float, self, 0x38) = transformedPoint[1];
    CSF_AT(float, self, 0x3C) = transformedPoint[2];

    CSF_AT(float, self, 0x40) = CSF_AT(float, self, 0x34);
    CSF_AT(float, self, 0x44) = CSF_AT(float, self, 0x38);
    CSF_AT(float, self, 0x48) = CSF_AT(float, self, 0x3C);

    const float deltaX =
        CSF_AT(float, self, 0x34) - CSF_AT(float, self, 0x28);
    const float deltaY =
        CSF_AT(float, self, 0x38) - CSF_AT(float, self, 0x2C);
    const float deltaZ =
        CSF_AT(float, self, 0x3C) - CSF_AT(float, self, 0x30);

    transformedPoint[0] =
        deltaX * g_BoundsInterpolationFraction + CSF_AT(float, self, 0x28);
    transformedPoint[1] =
        deltaY * g_BoundsInterpolationFraction + CSF_AT(float, self, 0x2C);
    transformedPoint[2] =
        deltaZ * g_BoundsInterpolationFraction + CSF_AT(float, self, 0x30);

    CSF_AT(float, self, 0x4C) = transformedPoint[0];
    CSF_AT(float, self, 0x50) = transformedPoint[1];
    CSF_AT(float, self, 0x54) = transformedPoint[2];
    CSF_AT(float, self, 0x50) =
        CSF_AT(float, self, 0x50) + g_BoundsPointHeightOffset;

    CSF_AT(float, self, 0x58) = 0.0f;
    CSF_AT(float, self, 0x5C) = 0.0f;
    CSF_AT(float, self, 0x60) = 0.0f;

    if (frameMatrix.pad_0004[0])
        RwMatrixDestroy(
            reinterpret_cast<std::uint32_t*>(frameMatrix.field_0000));

    char result = static_cast<char>(
        CSF_AT(uint8_t, inverseMatrixStorage, 4));
    if (result)
    {
        result = static_cast<char>(RwMatrixDestroy(
            reinterpret_cast<std::uint32_t*>(
                CSF_AT(float*, inverseMatrixStorage, 0))));
    }
    return result;
}

HOOK(0x00409930, C_0082d844_InitializeLocalReferencePoints, "auto");
