#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RpWorldAddCamera, 0x0058DD90, (int a1, int a2));
CSF_FUNC(int, __cdecl, RpWorldRemoveCamera, 0x0058DDC0, (int a1, int a2));
CSF_FUNC(int, __cdecl, RwCameraGetWorld, 0x0058DDF0, (int a1));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(int, __cdecl, RwCameraBeginUpdate, 0x00628E50, (int camera));
CSF_FUNC(int, __cdecl, RwCameraClear, 0x00628F60, (int camera, int color, int flags));
CSF_FUNC(int, __fastcall, SceneLoader_GetSceneController, 0x00652010, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, S_00526d40_ConvertDirection, 0x006ABA10, (S_00526d40* self, void* , uint32_t* output));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, GfxFrames_OptimizeFrameMatrix, 0x006BF2D0, (void* self, void*));
CSF_FUNC(int, __fastcall, GfxFrames_OrthoNormalizeOwnMatrix, 0x006BF2F0, (void* self, void*));
CSF_FUNC(int, __fastcall, C_00860bb8_GetLoadedScene, 0x006CBAC0, (C_00860bb8* self, void*));

CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);
CSF_GLOBAL(float, g_ColorChannelScale, 0x0082FEA8);
CSF_GLOBAL(float, g_CameraProjectionScale, 0x00861038);

static bool __fastcall CCamera_BeginCameraUpdate(
    CCamera* self, void* /*edx*/, int clearFlags,
    float red, float green, float blue, float alpha)
{
    void* globalContext = reinterpret_cast<void*>(GetGlobalInt());
    auto* sceneController = reinterpret_cast<C_00860bb8*>(
        SceneLoader_GetSceneController(globalContext, nullptr));
    const int loadedWorld = C_00860bb8_GetLoadedScene(sceneController, nullptr);
    const int cameraWorld = RwCameraGetWorld(static_cast<int>(self->field_000c));

    if (cameraWorld != loadedWorld && loadedWorld)
    {
        if (cameraWorld)
            RpWorldRemoveCamera(cameraWorld, static_cast<int>(self->field_000c));
        RpWorldAddCamera(loadedWorld, static_cast<int>(self->field_000c));
    }

    void* frame = reinterpret_cast<std::uint8_t*>(self) + 0x10;
    GfxFrames_OptimizeFrameMatrix(frame, nullptr);
    if (!self->field_0064)
        GfxFrames_OrthoNormalizeOwnMatrix(frame, nullptr);
    self->field_0064 =
        static_cast<std::int32_t>(self->field_0064 + 1u) % 128;

    bool succeeded = true;
    if (clearFlags)
    {
        std::uint32_t packedColor = static_cast<std::uint32_t>(
            static_cast<std::int64_t>(alpha * g_ColorChannelScale));
        packedColor = (packedColor << 8) | static_cast<std::uint32_t>(
            static_cast<std::int64_t>(red * g_ColorChannelScale));
        packedColor = (packedColor << 8) | static_cast<std::uint32_t>(
            static_cast<std::int64_t>(green * g_ColorChannelScale));
        packedColor = (packedColor << 8) | static_cast<std::uint32_t>(
            static_cast<std::int64_t>(blue * g_ColorChannelScale));

        std::uint8_t color[4];
        color[0] = static_cast<std::uint8_t>(packedColor >> 16);
        color[1] = static_cast<std::uint8_t>(packedColor >> 8);
        color[2] = static_cast<std::uint8_t>(packedColor);
        color[3] = static_cast<std::uint8_t>(packedColor >> 24);
        succeeded = RwCameraClear(
            static_cast<int>(self->field_000c),
            reinterpret_cast<int>(color), clearFlags) != 0;
    }
    if (succeeded)
        succeeded = RwCameraBeginUpdate(static_cast<int>(self->field_000c)) != 0;

    alignas(8) std::uint8_t matrixResultBytes[0x8];
    reinterpret_cast<void(__fastcall*)(void*, void*, void*)>(
        reinterpret_cast<void**>(self->field_0010)[0x8C / 4])(
            frame, nullptr, matrixResultBytes);

    auto* matrix = CSF_AT(S_00526d40*, matrixResultBytes, 0);
    self->field_0068 = CSF_AT(float, matrix, 0x30);
    self->field_006c = CSF_AT(float, matrix, 0x34);
    self->field_0070 = CSF_AT(float, matrix, 0x38);

    self->field_0074 = CSF_AT(std::uint32_t, self, 0x68);
    self->field_0078 = CSF_AT(std::uint32_t, self, 0x6C);
    self->field_007c = CSF_AT(std::uint32_t, self, 0x70);

    alignas(8) std::uint8_t directionBytes[0xC];
    auto* directionOutput = reinterpret_cast<std::uint32_t*>(directionBytes);
    auto* direction = S_00526d40_ConvertDirection(
        CSF_AT(S_00526d40*, matrixResultBytes, 0), nullptr, directionOutput);
    self->field_0080 = direction[0];
    self->field_0084 = direction[1];
    const std::uint32_t forwardZ = direction[2];
    self->field_009c = 1;
    self->field_0088 = forwardZ;

    self->field_00a0 = CSF_AT(std::uint32_t, self, 0x68);
    self->field_00a4 = CSF_AT(std::uint32_t, self, 0x6C);
    self->field_00a8 = CSF_AT(std::uint32_t, self, 0x70);
    direction = S_00526d40_ConvertDirection(
        reinterpret_cast<S_00526d40*>(
            reinterpret_cast<std::uint8_t*>(
                CSF_AT(S_00526d40*, matrixResultBytes, 0)) + 0x10),
        nullptr, directionOutput);
    self->field_00ac = direction[0];
    self->field_00b0 = direction[1];
    self->field_00b4 = direction[2];
    self->field_00c8 = 1;
    CSF_AT(std::uint32_t, self, 0xCC) = 0;
    CSF_AT(std::uint32_t, self, 0xD0) = 1;

    const std::int32_t viewportWidth =
        static_cast<std::int32_t>(self->field_0040 - self->field_0038);
    const float projectionRatio = static_cast<float>(viewportWidth) /
        CSF_AT(float, reinterpret_cast<void*>(self->field_000c), 0x6C);
    const bool destroyMatrix = CSF_AT(std::uint8_t, matrixResultBytes, 4) != 0;
    const float inverseSquaredRatio =
        g_TransitionOne / (projectionRatio * projectionRatio);
    const float aspect = self->field_0048;
    self->field_00e0 =
        inverseSquaredRatio * aspect * aspect * g_CameraProjectionScale;
    self->field_00dc = inverseSquaredRatio * g_CameraProjectionScale;

    if (destroyMatrix)
        RwMatrixDestroy(reinterpret_cast<std::uint32_t*>(
            CSF_AT(S_00526d40*, matrixResultBytes, 0)));
    return succeeded;
}

HOOK(0x006D3010, CCamera_BeginCameraUpdate, "auto");
