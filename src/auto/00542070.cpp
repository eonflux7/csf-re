#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "globals.h"

CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void*, __fastcall, GetPreviewRenderer, 0x006B13B0,
         (void* self, void* edx));
CSF_FUNC(void*, __fastcall, GetSceneManager, 0x00652020,
         (void* self, void* edx));

CSF_GLOBAL(float, g_PreviewLeftScale, 0x008434F4);
CSF_GLOBAL(float, g_PreviewTopScale, 0x0082DD68);
CSF_GLOBAL(float, g_PreviewRightScale, 0x00833850);
CSF_GLOBAL(float, g_PreviewBottomScale, 0x008434F0);
CSF_GLOBAL(std::uint32_t, g_PreviewCameraAngleBits, 0x00860DAC);
CSF_GLOBAL(float, g_ColorWhite, 0x008A7D90);
CSF_GLOBAL(std::uint32_t, g_ColorBlack, 0x008A7DA0);
CSF_GLOBAL(std::uint32_t, g_ColorBlack_G, 0x008A7DA4);
CSF_GLOBAL(std::uint32_t, g_ColorBlack_B, 0x008A7DA8);
CSF_GLOBAL(std::uint32_t, g_ColorBlack_A, 0x008A7DAC);

static int __fastcall MenuMpCharacterSelect_CreatePreview(
    void* self, void* /*edx*/, void* control)
{
    ((void(__fastcall*)(void*, void*, void*))
        CSF_AT(void**, self, 0)[0x108 / 4])(self, nullptr, control);
    ((void(__fastcall*)(void*, void*, int))
        CSF_AT(void**, self, 0)[0xC8 / 4])(self, nullptr, 1);

    void* size = ((void*(__fastcall*)(void*, void*))
        CSF_AT(void**, control, 0)[0xA4 / 4])(control, nullptr);
    const std::int32_t width = CSF_AT(std::int32_t, size, 0);

    size = ((void*(__fastcall*)(void*, void*))
        CSF_AT(void**, control, 0)[0xA4 / 4])(control, nullptr);
    const std::int32_t height = CSF_AT(std::int32_t, size, 4);

    void* position = ((void*(__fastcall*)(void*, void*))
        CSF_AT(void**, control, 0)[0x9C / 4])(control, nullptr);
    const float roundedWidth = static_cast<float>(width);
    const std::uint32_t leftAdjustment = static_cast<std::uint32_t>(
        static_cast<std::int64_t>(
            static_cast<double>(width) * static_cast<double>(g_PreviewLeftScale)));
    const std::uint32_t left =
        CSF_AT(std::uint32_t, position, 0) - leftAdjustment;
    const float roundedHeight = static_cast<float>(height);

    position = ((void*(__fastcall*)(void*, void*))
        CSF_AT(void**, control, 0)[0x9C / 4])(control, nullptr);
    const std::uint32_t topAdjustment = static_cast<std::uint32_t>(
        static_cast<std::int64_t>(
            static_cast<double>(roundedHeight) *
            static_cast<double>(g_PreviewTopScale)));
    const std::uint32_t top =
        CSF_AT(std::uint32_t, position, 4) - topAdjustment;

    position = ((void*(__fastcall*)(void*, void*))
        CSF_AT(void**, control, 0)[0x9C / 4])(control, nullptr);
    const std::uint32_t rightAdjustment = static_cast<std::uint32_t>(
        static_cast<std::int64_t>(
            static_cast<double>(roundedWidth) *
            static_cast<double>(g_PreviewRightScale)));
    const std::uint32_t right =
        CSF_AT(std::uint32_t, position, 0) - rightAdjustment;

    position = ((void*(__fastcall*)(void*, void*))
        CSF_AT(void**, control, 0)[0x9C / 4])(control, nullptr);
    const std::uint32_t bottomAdjustment = static_cast<std::uint32_t>(
        static_cast<std::int64_t>(
            static_cast<double>(roundedHeight) *
            static_cast<double>(g_PreviewBottomScale)));
    const std::uint32_t bottom =
        CSF_AT(std::uint32_t, position, 4) - bottomAdjustment;

    struct PreviewDescription {
        std::uint32_t values[9];
        std::uint8_t enabled;
        std::uint8_t secondaryFlag;
    } preview;

    preview.values[1] = left;
    const std::uint32_t blackRed = g_ColorBlack;
    preview.values[3] = right;
    const std::uint32_t blackBlue = g_ColorBlack_B;
    preview.values[4] = bottom;
    const std::uint32_t blackGreen = g_ColorBlack_G;
    preview.values[5] = blackRed;
    const std::uint32_t blackAlpha = g_ColorBlack_A;
    preview.values[0] = 7;
    preview.enabled = 1;
    preview.secondaryFlag = 0;
    preview.values[2] = top;
    preview.values[6] = blackGreen;
    preview.values[7] = blackBlue;
    preview.values[8] = blackAlpha;

    void* renderer = GetPreviewRenderer(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    void* previewObject = ((void*(__fastcall*)(void*, void*, void*))
        CSF_AT(void**, renderer, 0)[0x18 / 4])(renderer, nullptr, &preview);

    int result = static_cast<int>(reinterpret_cast<std::uintptr_t>(previewObject));
    CSF_AT(std::uint32_t, self, 0x134) =
        static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(previewObject));

    if (previewObject) {
        std::uint32_t cameraDescription[9];
        const std::uint32_t cameraAngle = g_PreviewCameraAngleBits;
        cameraDescription[0] = 0;
        cameraDescription[1] = 0;
        cameraDescription[2] = 0;
        cameraDescription[3] = 0;
        cameraDescription[6] = 0;
        cameraDescription[7] = cameraAngle;
        cameraDescription[4] = 0x41200000;
        cameraDescription[5] = 0x461C4000;
        cameraDescription[8] = 0xFFFFFFFF;

        renderer = GetPreviewRenderer(
            reinterpret_cast<void*>(GetGlobalInt()), nullptr);
        void* camera = ((void*(__fastcall*)(void*, void*, void*))
            CSF_AT(void**, renderer, 0)[0x10 / 4])(
                renderer, nullptr, cameraDescription);
        CSF_AT(std::uint32_t, self, 0x138) =
            static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(camera));

        ((void(__fastcall*)(void*, void*, std::uint32_t))
            CSF_AT(void**, camera, 0)[0x20 / 4])(camera, nullptr, 0x3F9C61AA);

        previewObject = reinterpret_cast<void*>(
            CSF_AT(std::uint32_t, self, 0x134));
        camera = reinterpret_cast<void*>(
            CSF_AT(std::uint32_t, self, 0x138));
        ((void(__fastcall*)(void*, void*, void*))
            CSF_AT(void**, previewObject, 0)[0x10 / 4])(
                previewObject, nullptr, camera);

        previewObject = reinterpret_cast<void*>(
            CSF_AT(std::uint32_t, self, 0x134));
        ((void(__fastcall*)(void*, void*, int, int))
            CSF_AT(void**, previewObject, 0)[0x4C / 4])(
                previewObject, nullptr, 1, 0);

        previewObject = reinterpret_cast<void*>(
            CSF_AT(std::uint32_t, self, 0x134));
        ((void(__fastcall*)(void*, void*, int, int))
            CSF_AT(void**, previewObject, 0)[0x4C / 4])(
                previewObject, nullptr, 2, 0);

        previewObject = reinterpret_cast<void*>(
            CSF_AT(std::uint32_t, self, 0x134));
        ((void(__fastcall*)(void*, void*, int, int))
            CSF_AT(void**, previewObject, 0)[0x4C / 4])(
                previewObject, nullptr, 4, 0);

        void* sceneManager = GetSceneManager(
            reinterpret_cast<void*>(GetGlobalInt()), nullptr);
        void* light = ((void*(__fastcall*)(void*, void*, int, int))
            CSF_AT(void**, sceneManager, 0)[0x28 / 4])(
                sceneManager, nullptr, 2, 3);
        CSF_AT(std::uint32_t, self, 0x148) =
            static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(light));

        ((void(__fastcall*)(void*, void*, float*))
            CSF_AT(void**, light, 0)[0x34 / 4])(
                light, nullptr, &g_ColorWhite);

        light = reinterpret_cast<void*>(CSF_AT(std::uint32_t, self, 0x148));
        ((void(__fastcall*)(void*, void*, std::uint32_t))
            CSF_AT(void**, light, 0)[0x3C / 4])(light, nullptr, 0x461C4000);

        light = reinterpret_cast<void*>(CSF_AT(std::uint32_t, self, 0x148));
        ((void(__fastcall*)(void*, void*, int))
            CSF_AT(void**, light, 0)[0x2C / 4])(light, nullptr, 0);

        light = reinterpret_cast<void*>(CSF_AT(std::uint32_t, self, 0x148));
        void* transform = ((void*(__fastcall*)(void*, void*))
            CSF_AT(void**, light, 0)[0x24 / 4])(light, nullptr);
        std::uint32_t lightPosition[3];
        lightPosition[0] = 0xC2C80000;
        lightPosition[1] = 0x43160000;
        lightPosition[2] = 0;
        result = ((int(__fastcall*)(void*, void*, void*))
            CSF_AT(void**, transform, 0)[0x14 / 4])(
                transform, nullptr, lightPosition);
    }

    CSF_AT(std::uint32_t, self, 0x154) = 0;
    CSF_AT(std::uint32_t, self, 0x150) = 0;
    CSF_AT(std::uint32_t, self, 0x14C) = 0xC0490FDB;
    return result;
}

HOOK(0x00542070, MenuMpCharacterSelect_CreatePreview, "auto");
