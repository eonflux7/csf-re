#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "globals.h"

CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void*, __fastcall, GetDisplayContext, 0x00652160,
         (void* self, void* edx));
CSF_FUNC(int, __fastcall, ForwardLayoutToChild, 0x00720DF0,
         (void* self, void* edx, void* layout));

CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);
CSF_GLOBAL(float, g_LayoutInset, 0x0082E0F0);
CSF_GLOBAL(std::uint32_t, g_ScreenWidth, 0x00899B9C);

static int __fastcall MenuMessageBox_UpdateHorizontalLayout(
    void* self, void* /*edx*/)
{
    void* control = CSF_AT(void*, self, 0x894);
    int result =
        ((int(__fastcall*)(void*, void*))CSF_AT(void**, control, 0)[115])(
            control, nullptr);

    if (!CSF_AT(void*, self, 0x58) || !result)
        return result;

    void* context = GetDisplayContext(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    ((void*(__fastcall*)(void*, void*))CSF_AT(void**, context, 0)[6])(
        context, nullptr);

    context = GetDisplayContext(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    void* dimensions =
        ((void*(__fastcall*)(void*, void*))CSF_AT(void**, context, 0)[6])(
            context, nullptr);

    double width = static_cast<double>(
        CSF_AT(std::int32_t, dimensions, 0));
    double screenWidth = static_cast<double>(
        static_cast<std::int32_t>(g_ScreenWidth));
    if (static_cast<std::int32_t>(g_ScreenWidth) < 0)
        screenWidth += static_cast<double>(g_UnsignedIntegerBias);

    float scale = static_cast<float>(width / screenWidth);
    std::uint32_t origin[2] = {0, 0};
    control = CSF_AT(void*, self, 0x58);
    ((void(__fastcall*)(void*, void*, std::uint32_t*))
        CSF_AT(void**, control, 0)[38])(control, nullptr, origin);

    std::uint32_t size[2] = {0, 0};
    context = GetDisplayContext(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    dimensions =
        ((void*(__fastcall*)(void*, void*))CSF_AT(void**, context, 0)[6])(
            context, nullptr);
    size[1] = CSF_AT(std::uint32_t, dimensions, 4);

    std::uint32_t inset = static_cast<std::uint32_t>(
        static_cast<std::int64_t>(
            static_cast<double>(scale) * static_cast<double>(g_LayoutInset)));
    size[0] = CSF_AT(std::uint32_t, self, 0x970) - inset;

    control = CSF_AT(void*, self, 0x58);
    ((void(__fastcall*)(void*, void*, std::uint32_t*))
        CSF_AT(void**, control, 0)[40])(control, nullptr, size);

    return ForwardLayoutToChild(
        CSF_AT(void*, self, 0x58), nullptr,
        static_cast<std::uint8_t*>(self) + 0x90);
}

HOOK(0x00529280, MenuMessageBox_UpdateHorizontalLayout, "auto");
