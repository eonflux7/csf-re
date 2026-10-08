#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "globals.h"

using PreviewAnimationTable = std::uint32_t[5];

CSF_GLOBAL(float, g_PreviewRotationScale, 0x00843284);
CSF_GLOBAL(PreviewAnimationTable, g_PreviewAnimations, 0x00843288);
CSF_GLOBAL(float, g_PreviewAnimationInterval, 0x0084329C);
CSF_GLOBAL(float, g_PreviewRadiansToDegrees, 0x008432A8);

CSF_FUNC(double, __cdecl, MenuMpCharacterSelect_NormalizeAngle,
         0x00406950, (float angle));

static void __fastcall MenuMpCharacterSelect_UpdatePreview(
    void* self, void* /*edx*/, float elapsedTime)
{
    void* preview = CSF_AT(void*, self, 0x13C);
    if (!preview)
        return;

    const double animationTime =
        static_cast<double>(elapsedTime) + CSF_AT(float, self, 0x154);
    CSF_AT(float, self, 0x154) = static_cast<float>(animationTime);

    if (animationTime > static_cast<double>(g_PreviewAnimationInterval))
    {
        const std::uint32_t animationIndex =
            (CSF_AT(std::uint32_t, self, 0x150) + std::uint32_t{1}) % 5u;
        CSF_AT(std::uint32_t, self, 0x154) = 0;
        CSF_AT(std::uint32_t, self, 0x150) = animationIndex;

        ((void(__fastcall*)(void*, void*, std::uint32_t, int, int, int))
             CSF_AT(void**, preview, 0)[0x8C / 4])(
            preview, nullptr, g_PreviewAnimations[animationIndex], 0, -1, 0);
    }

    preview = CSF_AT(void*, self, 0x13C);
    const float rotationStep = static_cast<float>(
        static_cast<double>(elapsedTime) * g_PreviewRotationScale);

    void* previewObject =
        ((void*(__fastcall*)(void*, void*))
             CSF_AT(void**, preview, 0)[0x28 / 4])(preview, nullptr);

    void* transform =
        ((void*(__fastcall*)(void*, void*))
             CSF_AT(void**, previewObject, 0)[0x18 / 4])(
            previewObject, nullptr);

    const float rotationDegrees = static_cast<float>(
        static_cast<double>(rotationStep) * g_PreviewRadiansToDegrees);
    ((void(__fastcall*)(void*, void*, float))
         CSF_AT(void**, transform, 0)[0x40 / 4])(
        transform, nullptr, rotationDegrees);

    const float angle = static_cast<float>(
        static_cast<double>(rotationStep) + CSF_AT(float, self, 0x14C));
    CSF_AT(float, self, 0x14C) = static_cast<float>(
        MenuMpCharacterSelect_NormalizeAngle(angle));
}

HOOK(0x00542420, MenuMpCharacterSelect_UpdatePreview, "auto");
