#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "globals.h"

CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void*, __fastcall, LookupObjectById, 0x00652160,
         (void* self, void* edx));

CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);
CSF_GLOBAL(float, g_LayoutInset, 0x0082FEBC);
CSF_GLOBAL(std::uint32_t, g_ScreenHeight, 0x00899BA0);

static int __fastcall MenuMessageBox_UpdateCombinedHeight(
    void* self, void* /*edx*/)
{
    void* source = CSF_AT(void*, self, 0x894);
    void* layout =
        ((void*(__fastcall*)(void*, void*))CSF_AT(void**, source, 0)[115])(
            source, nullptr);

    void* control = CSF_AT(void*, self, 0x58);
    if (!control || !CSF_AT(void*, self, 0x5C) || !layout)
        return static_cast<int>(reinterpret_cast<std::uintptr_t>(control));

    void* context = LookupObjectById(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    void* dimensions =
        ((void*(__fastcall*)(void*, void*))CSF_AT(void**, context, 0)[6])(
            context, nullptr);

    double height = static_cast<double>(
        CSF_AT(std::int32_t, dimensions, 4));
    double screenHeight = static_cast<double>(
        static_cast<std::int32_t>(g_ScreenHeight));
    if (static_cast<std::int32_t>(g_ScreenHeight) < 0)
        screenHeight += static_cast<double>(g_UnsignedIntegerBias);
    float scale = static_cast<float>(height / screenHeight);

    control = CSF_AT(void*, self, 0x58);
    void* child =
        ((void*(__fastcall*)(void*, void*))CSF_AT(void**, control, 0)[68])(
            control, nullptr);
    std::uint32_t mainHeight = 0;
    if (child)
    {
        mainHeight =
            ((std::uint32_t(__fastcall*)(void*, void*))
                CSF_AT(void**, child, 0)[40])(child, nullptr);
    }

    control = CSF_AT(void*, self, 0x5C);
    child =
        ((void*(__fastcall*)(void*, void*))CSF_AT(void**, control, 0)[68])(
            control, nullptr);
    std::uint32_t additionalHeight = 0;
    if (child)
    {
        additionalHeight =
            ((std::uint32_t(__fastcall*)(void*, void*))
                CSF_AT(void**, child, 0)[40])(child, nullptr);
    }

    if (CSF_AT(std::uint8_t, self, 0x8B8))
    {
        std::uint32_t inset = static_cast<std::uint32_t>(
            static_cast<std::int64_t>(
                static_cast<double>(scale)
                * static_cast<double>(g_LayoutInset)));
        additionalHeight -= inset;
    }
    else
    {
        additionalHeight = 0;
    }

    void* layoutDimensions =
        ((void*(__fastcall*)(void*, void*))CSF_AT(void**, layout, 0)[8])(
            layout, nullptr);
    std::uint32_t inset = static_cast<std::uint32_t>(
        static_cast<std::int64_t>(
            static_cast<double>(scale) * static_cast<double>(g_LayoutInset)));

    std::uint32_t combinedHeight =
        CSF_AT(std::uint32_t, layoutDimensions, 4) - (inset << 1);
    combinedHeight += additionalHeight;
    combinedHeight += mainHeight;
    CSF_AT(std::uint32_t, self, 0x964) = combinedHeight;

    return static_cast<int>(mainHeight);
}

HOOK(0x00529A90, MenuMessageBox_UpdateCombinedHeight, "auto");
