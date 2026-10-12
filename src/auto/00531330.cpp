#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_488830, 0x00488830, (void* self, void* edx, void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(uint32_t*, __fastcall, MpServerInfo_InitDefaultStyle, 0x00530AB0, (uint32_t* self, void*));
CSF_FUNC(void*, __fastcall, LookupObjectById, 0x00652160, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());

CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);
CSF_GLOBAL(std::uint32_t, g_ScreenWidth, 0x00899B9C);
CSF_GLOBAL(std::uint32_t, g_ScreenHeight, 0x00899BA0);

static void* __stdcall MenuMpServerInfo_CreateStyledElement(
    int elementId,
    const std::uint32_t* bounds,
    void* menu,
    void* text,
    int styleId)
{
    // The style and its trailing string/scale data occupy one contiguous frame.
    alignas(8) std::uint8_t styleBytes[0x44];
    auto* style = reinterpret_cast<std::uint32_t*>(styleBytes);
    void* styleText = styleBytes + 0x18;

    MpServerInfo_InitDefaultStyle(style, nullptr);
    CSF_AT(std::uint32_t, styleBytes, 0x08) = bounds[0];
    CSF_AT(std::uint32_t, styleBytes, 0x0C) = bounds[1];
    CSF_AT(std::uint32_t, styleBytes, 0x10) = bounds[2];
    CSF_AT(std::uint32_t, styleBytes, 0x14) = bounds[3];

    AssignStringRange_488830(styleText, nullptr, text, 0, 0xFFFFFFFF);
    CSF_AT(int, styleBytes, 0x00) = styleId;

    void* object = LookupObjectById(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    void* dimensions =
        ((void* (__fastcall*)(void*, void*))
            CSF_AT(void**, object, 0)[0x18 / 4])(object, nullptr);
    const std::uint32_t width = CSF_AT(std::uint32_t, dimensions, 0x00);

    object = LookupObjectById(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    dimensions =
        ((void* (__fastcall*)(void*, void*))
            CSF_AT(void**, object, 0)[0x18 / 4])(object, nullptr);
    const std::uint32_t height = CSF_AT(std::uint32_t, dimensions, 0x04);

    CSF_AT(float, styleText, 0x24) = (float)width / (float)g_ScreenWidth;
    CSF_AT(float, styleText, 0x28) = (float)height / (float)g_ScreenHeight;
    CSF_AT(float, styleText, 0x1C) = CSF_AT(float, styleText, 0x24);
    CSF_AT(float, styleText, 0x20) = CSF_AT(float, styleText, 0x28);

    void* result =
        ((void* (__fastcall*)(void*, void*, int, std::uint32_t*, const char*))
            CSF_AT(void**, menu, 0)[0x258 / 4])(
                menu, nullptr, elementId, style,
                CSF_STR(0x0082FFA4, "Menus/Ingame_01.fnt"));

    if (CSF_AT(std::uint32_t, styleText, 0x18) >= 8)
    {
        result = CSF_AT(void*, styleText, 0x04);
        if (result)
            return reinterpret_cast<void*>(
                static_cast<std::uintptr_t>(Mem_FreeDebug(result)));
    }
    return result;
}

HOOK(0x00531330, MenuMpServerInfo_CreateStyledElement, "auto");
