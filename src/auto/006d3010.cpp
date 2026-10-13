#include <cstdint>
#include "globals.h"
#include "hook.h"
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

static bool __fastcall GfxCamera_BeginCameraUpdate(
    void* self, void* /*edx*/, int clearFlags,
    float red, float green, float blue, float alpha)
{
    void* globalState = reinterpret_cast<void*>(GetGlobalInt());
    auto* sceneController = reinterpret_cast<C_00860bb8*>(
        SceneLoader_GetSceneController(globalState, nullptr));
    const int loadedScene = C_00860bb8_GetLoadedScene(sceneController, nullptr);
    const int currentWorld = RwCameraGetWorld(CSF_AT(int, self, 0x0C));

    if (currentWorld != loadedScene && loadedScene) {
        if (currentWorld)
            RpWorldRemoveCamera(currentWorld, CSF_AT(int, self, 0x0C));
        RpWorldAddCamera(loadedScene, CSF_AT(int, self, 0x0C));
    }

    void* frame = static_cast<std::uint8_t*>(self) + 0x10;
    GfxFrames_OptimizeFrameMatrix(frame, nullptr);
    if (!CSF_AT(int, self, 0x64))
        GfxFrames_OrthoNormalizeOwnMatrix(frame, nullptr);
    CSF_AT(int, self, 0x64) =
        static_cast<std::int32_t>(CSF_AT(std::uint32_t, self, 0x64) + 1) % 128;

    bool succeeded = true;
    if (clearFlags) {
        std::uint32_t packed =
            static_cast<std::uint32_t>(static_cast<std::int64_t>(alpha * g_ColorChannelScale));
        packed <<= 8;
        packed |= static_cast<std::uint32_t>(
            static_cast<std::int64_t>(red * g_ColorChannelScale));
        packed <<= 8;
        packed |= static_cast<std::uint32_t>(
            static_cast<std::int64_t>(green * g_ColorChannelScale));
        packed <<= 8;
        packed |= static_cast<std::uint32_t>(
            static_cast<std::int64_t>(blue * g_ColorChannelScale));

        std::uint8_t color[4] = {
            static_cast<std::uint8_t>(packed >> 16),
            static_cast<std::uint8_t>(packed >> 8),
            static_cast<std::uint8_t>(packed),
            static_cast<std::uint8_t>(packed >> 24)
        };
        succeeded = RwCameraClear(
            CSF_AT(int, self, 0x0C), reinterpret_cast<int>(color), clearFlags) != 0;
    }
    if (succeeded)
        succeeded = RwCameraBeginUpdate(CSF_AT(int, self, 0x0C)) != 0;

    alignas(8) std::uint8_t matrixResultBytes[0x8] = {};
    ((void(__fastcall*)(void*, void*, void*))
        CSF_AT(void**, frame, 0)[0x8C / 4])(frame, nullptr, matrixResultBytes);

    void* matrix = CSF_AT(S_00526d40*, matrixResultBytes, 0);
    CSF_AT(std::uint32_t, self, 0x68) = CSF_AT(std::uint32_t, matrix, 0x30);
    CSF_AT(std::uint32_t, self, 0x6C) = CSF_AT(std::uint32_t, matrix, 0x34);
    CSF_AT(std::uint32_t, self, 0x70) = CSF_AT(std::uint32_t, matrix, 0x38);

    CSF_AT(std::uint32_t, self, 0x74) = CSF_AT(std::uint32_t, self, 0x68);
    CSF_AT(std::uint32_t, self, 0x78) = CSF_AT(std::uint32_t, self, 0x6C);
    CSF_AT(std::uint32_t, self, 0x7C) = CSF_AT(std::uint32_t, self, 0x70);

    std::uint32_t directionBuffer[3] = {};
    std::uint32_t* direction = S_00526d40_ConvertDirection(
        CSF_AT(S_00526d40*, matrixResultBytes, 0), nullptr, directionBuffer);
    CSF_AT(std::uint32_t, self, 0x80) = direction[0];
    CSF_AT(std::uint32_t, self, 0x84) = direction[1];
    const std::uint32_t lastDirectionComponent = direction[2];
    CSF_AT(std::uint8_t, self, 0x9C) = 1;
    CSF_AT(std::uint32_t, self, 0x88) = lastDirectionComponent;

    CSF_AT(std::uint32_t, self, 0xA0) = CSF_AT(std::uint32_t, self, 0x68);
    CSF_AT(std::uint32_t, self, 0xA4) = CSF_AT(std::uint32_t, self, 0x6C);
    CSF_AT(std::uint32_t, self, 0xA8) = CSF_AT(std::uint32_t, self, 0x70);

    direction = S_00526d40_ConvertDirection(
        reinterpret_cast<S_00526d40*>(
            reinterpret_cast<std::uint8_t*>(
                CSF_AT(S_00526d40*, matrixResultBytes, 0)) + 0x10),
        nullptr, directionBuffer);
    CSF_AT(std::uint32_t, self, 0xAC) = direction[0];
    CSF_AT(std::uint32_t, self, 0xB0) = direction[1];
    CSF_AT(std::uint32_t, self, 0xB4) = direction[2];
    CSF_AT(std::uint8_t, self, 0xC8) = 1;
    CSF_AT(int, self, 0xCC) = 0;
    CSF_AT(int, self, 0xD0) = 1;

    const std::uint32_t left = CSF_AT(std::uint32_t, self, 0x38);
    const std::int32_t width =
        static_cast<std::int32_t>(CSF_AT(std::uint32_t, self, 0x40) - left);
    void* camera = reinterpret_cast<void*>(CSF_AT(int, self, 0x0C));
    const bool destroyMatrix = CSF_AT(std::uint8_t, matrixResultBytes, 0x04) != 0;
    const float projectionRatio = static_cast<float>(width) / CSF_AT(float, camera, 0x6C);
    const float cameraScale = CSF_AT(float, self, 0x48);
    const float inverseSquare = g_TransitionOne / (projectionRatio * projectionRatio);
    CSF_AT(float, self, 0xE0) =
        inverseSquare * cameraScale * cameraScale * g_CameraProjectionScale;
    CSF_AT(float, self, 0xDC) = inverseSquare * g_CameraProjectionScale;

    if (destroyMatrix)
        RwMatrixDestroy(CSF_AT(std::uint32_t*, matrixResultBytes, 0));
    return succeeded;
}

HOOK(0x006D3010, GfxCamera_BeginCameraUpdate, "auto");
