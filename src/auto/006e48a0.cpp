#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwFrameTranslate, 0x00623830, (int a1, int a2, int a3));
CSF_FUNC(int, __cdecl, RwCameraSetNearClipPlane, 0x00628E90, (int a1, float a2));
CSF_FUNC(int, __cdecl, RwCameraSetFarClipPlane, 0x00628EC0, (int a1, float a2));
CSF_FUNC(int, __cdecl, RwCameraSetProjection, 0x00628F90, (int a1, int a2));
CSF_FUNC(int, __cdecl, RwCameraSetViewWindow, 0x00629000, (int a1, uint32_t* a2));

CSF_GLOBAL(float, g_SceneLoadMillisecondsToSeconds, 0x0082D4F0);
CSF_GLOBAL(float, g_MessageBoxSizeScale, 0x00831430);

static int __fastcall S_006e48a0_ConfigureCameraView(
    S_006e48a0* self, void* /*edx*/, int camera, int object, float viewSize)
{
    RwCameraSetProjection(camera, 2);
    RwCameraSetFarClipPlane(camera, viewSize + viewSize);
    RwCameraSetNearClipPlane(
        camera, viewSize * g_SceneLoadMillisecondsToSeconds);

    alignas(8) uint8_t viewWindowBytes[0xC]{};
    CSF_AT(float, viewWindowBytes, 0x0) = viewSize;
    CSF_AT(float, viewWindowBytes, 0x4) = viewSize;
    RwCameraSetViewWindow(
        camera, reinterpret_cast<uint32_t*>(viewWindowBytes));

    RwCameraSetFarClipPlane(camera, 20000.0f);
    RwCameraSetNearClipPlane(camera, 10.0f);

    const int frame = CSF_AT(int, reinterpret_cast<void*>(camera), 0x4);
    std::memcpy(
        reinterpret_cast<uint8_t*>(frame) + 0x10,
        CSF_AT(const void*, reinterpret_cast<void*>(object), 0x40),
        0x40);

    alignas(8) uint8_t translationBytes[0xC];
    CSF_AT(uint32_t, translationBytes, 0x0) = self->field_0004;
    CSF_AT(float, translationBytes, 0x4) = self->field_0008;
    CSF_AT(float, translationBytes, 0x8) = CSF_AT(float, self, 0xC);
    RwFrameTranslate(frame, reinterpret_cast<int>(translationBytes), 2);

    CSF_AT(float, translationBytes, 0x0) = 0.0f;
    CSF_AT(float, translationBytes, 0x4) = 0.0f;
    CSF_AT(float, translationBytes, 0x8) =
        viewSize * g_MessageBoxSizeScale;
    return RwFrameTranslate(
        frame, reinterpret_cast<int>(translationBytes), 1);
}

HOOK(0x006E48A0, S_006e48a0_ConfigureCameraView, "auto");
