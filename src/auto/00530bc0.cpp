#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, MpServerInfo_InitDefaultStyle, 0x00530AB0, (uint32_t* self, void*));
CSF_FUNC(void*, __fastcall, LookupObjectById, 0x00652160, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, MenuMessageBox_ResolveEntryByKey, 0x00692440, (int code, int flag));
CSF_FUNC(std::uint32_t*, __fastcall, MenuProgressInput_AssignText, 0x00696F90, (std::uint32_t* self, void* edx, std::int16_t* text));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());

CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);
CSF_GLOBAL(unsigned int, g_ScreenWidth, 0x00899B9C);
CSF_GLOBAL(unsigned int, g_ScreenHeight, 0x00899BA0);

static void* __stdcall MenuMpServerInfo_CreateStyledText(
    int itemId, std::uint32_t* rectangle, void* menu, int textKey, int styleId)
{
    // The style header and text storage form one contiguous call argument.
    alignas(8) std::uint8_t styleBytes[0x44];
    auto* style = reinterpret_cast<std::uint32_t*>(styleBytes);
    auto* textStorage = reinterpret_cast<std::uint32_t*>(styleBytes + 0x18);

    MpServerInfo_InitDefaultStyle(style, nullptr);
    style[2] = rectangle[0];
    style[3] = rectangle[1];
    style[4] = rectangle[2];
    style[5] = rectangle[3];

    auto* text = reinterpret_cast<std::int16_t*>(
        MenuMessageBox_ResolveEntryByKey(textKey, 2));
    MenuProgressInput_AssignText(textStorage, nullptr, text);
    style[0] = styleId;

    void* object = LookupObjectById(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    void* dimensions =
        (reinterpret_cast<void*(__fastcall*)(void*, void*)>(
            CSF_AT(void**, object, 0)[0x18 / 4]))(object, nullptr);
    const std::uint32_t width = CSF_AT(std::uint32_t, dimensions, 0);

    object = LookupObjectById(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    dimensions =
        (reinterpret_cast<void*(__fastcall*)(void*, void*)>(
            CSF_AT(void**, object, 0)[0x18 / 4]))(object, nullptr);
    const std::uint32_t height = CSF_AT(std::uint32_t, dimensions, 4);

    CSF_AT(float, styleBytes, 0x3C) =
        static_cast<float>(width) / static_cast<float>(g_ScreenWidth);
    CSF_AT(float, styleBytes, 0x40) =
        static_cast<float>(height) / static_cast<float>(g_ScreenHeight);
    CSF_AT(float, styleBytes, 0x34) = CSF_AT(float, styleBytes, 0x3C);
    CSF_AT(float, styleBytes, 0x38) = CSF_AT(float, styleBytes, 0x40);

    void* result =
        (reinterpret_cast<void*(__fastcall*)(
            void*, void*, int, std::uint32_t*, const char*)>(
                CSF_AT(void**, menu, 0)[0x258 / 4]))(
                    menu, nullptr, itemId, style,
                    CSF_STR(0x0082FFA4, "Menus/Ingame_01.fnt"));

    if (CSF_AT(std::uint32_t, styleBytes, 0x30) >= 8)
    {
        result = CSF_AT(void*, styleBytes, 0x1C);
        if (result)
            return reinterpret_cast<void*>(Mem_FreeDebug(result));
    }
    return result;
}

HOOK(0x00530BC0, MenuMpServerInfo_CreateStyledText, "auto");
