#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __stdcall, MenuMpServerInfo_SetLocalizedHeader, 0x00530BC0, (int a1, uint32_t* a2, int a3, int a4, int a5));
CSF_FUNC(void*, __stdcall, MenuMpServerInfo_SetStringHeader, 0x00531330, (int a1, uint32_t* arg4, int a3, void* a2, int a5));
CSF_FUNC(int, __cdecl, GetGlobal8B0688, 0x00550CC0, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_558150, 0x00558150, ());
CSF_FUNC(void*, __fastcall, LookupObjectById, 0x00652160, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, MenuMessageBox_ResolveEntryByKey, 0x00692440, (int code, int flag));
CSF_FUNC(void, __cdecl, MenuLoading_CopyLocalizedText, 0x006956C0, (uint16_t* destination, const uint16_t* source, int capacity));
CSF_FUNC(void*, __cdecl, FormatCountTextBuffer, 0x00696A80, (void* outString));
CSF_FUNC(void*, __fastcall, ConstructWideString, 0x00696F50, (void* self, void* edx, wchar_t* String));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, CGuiListBoxBase_SetItemHeight, 0x00714750, (CGuiListBoxBase* self, void* , int itemHeight));
CSF_FUNC(void, __fastcall, CGuiListBoxBase_EnsureColumn, 0x00716490, (CGuiListBoxBase* self, void* , std::int32_t columnIndex));
CSF_FUNC(void, __fastcall, CGuiListBoxBase_SetColumnWidth, 0x00716520, (CGuiListBoxBase* self, void* , std::uint32_t columnIndex, std::int32_t width));
CSF_FUNC(void*, __fastcall, ForwardToStoredObjectVirtualSlot40, 0x00722DF0, (void* self, void* edx));

CSF_GLOBAL(float, g_MenuWidthScale, 0x0083F24C);
CSF_GLOBAL(float, g_ServerInfoFourthColumnWidth, 0x00841B44);
CSF_GLOBAL(float, g_ServerInfoStatisticColumnWidth, 0x00841B48);
CSF_GLOBAL(float, g_ServerInfoNameColumnWidth, 0x00841B4C);
CSF_GLOBAL(float, g_ServerInfoFirstColumnWidth, 0x00841B50);
CSF_GLOBAL(int, g_NumberFormattingMode, 0x0085E684);

// The formatter consumes a VC7.1 wide string by value, followed by
// printf-style arguments. This POD preserves that stack argument layout.
struct MenuMpServerInfo_WideStringArgument {
    uint32_t words[7];
};

static void MenuMpServerInfo_ClearList(uint32_t listAddress)
{
    void* list = reinterpret_cast<void*>(listAddress);
    reinterpret_cast<void(__fastcall*)(void*, void*)>(
        CSF_AT(void**, list, 0)[0x298 / 4])(list, nullptr);
}

static void MenuMpServerInfo_UpdateItemHeight(uint32_t listAddress)
{
    auto* list = reinterpret_cast<CGuiListBoxBase*>(listAddress);
    void* font = ForwardToStoredObjectVirtualSlot40(list, nullptr);
    if (font) {
        int height = reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, font, 0)[0x18 / 4])(font, nullptr, 1);
        CGuiListBoxBase_SetItemHeight(list, nullptr, height);
    }
}

static void MenuMpServerInfo_SetColumnWidths(uint32_t& listAddress)
{
    for (uint32_t column = 0; column < 6; ++column) {
        void* registry = reinterpret_cast<void*>(GetGlobalInt());
        void* setting = LookupObjectById(registry, nullptr);
        int* screenWidth = reinterpret_cast<int*(__fastcall*)(void*, void*)>(
            CSF_AT(void**, setting, 0)[0x18 / 4])(setting, nullptr);

        float baseWidth;
        switch (column) {
        case 0: baseWidth = g_ServerInfoFirstColumnWidth; break;
        case 1: baseWidth = g_ServerInfoNameColumnWidth; break;
        case 3: baseWidth = g_ServerInfoFourthColumnWidth; break;
        default: baseWidth = g_ServerInfoStatisticColumnWidth; break;
        }

        int width = static_cast<int>(
            static_cast<float>(*screenWidth) * g_MenuWidthScale * baseWidth);
        CGuiListBoxBase_SetColumnWidth(
            reinterpret_cast<CGuiListBoxBase*>(listAddress),
            nullptr, column, width);
    }
}

static void MenuMpServerInfo_ReadHeaderColor(void* color, int team)
{
    void* blueComponentSettings =
        reinterpret_cast<void*>(GetGlobalValue_558150());
    void* greenComponentSettings =
        reinterpret_cast<void*>(GetGlobalValue_558150());
    void* redComponentSettings =
        reinterpret_cast<void*>(GetGlobalValue_558150());

    uint8_t red;
    uint8_t green;
    uint8_t blue;
    if (team == 0) {
        red = CSF_AT(uint8_t, redComponentSettings, 466);
        green = CSF_AT(uint8_t, greenComponentSettings, 467);
        blue = CSF_AT(uint8_t, blueComponentSettings, 468);
    } else if (team == 1) {
        red = CSF_AT(uint8_t, redComponentSettings, 460);
        green = CSF_AT(uint8_t, greenComponentSettings, 461);
        blue = CSF_AT(uint8_t, blueComponentSettings, 462);
    } else {
        red = CSF_AT(uint8_t, redComponentSettings, 463);
        green = CSF_AT(uint8_t, greenComponentSettings, 464);
        blue = CSF_AT(uint8_t, blueComponentSettings, 465);
    }

    CSF_AT(float, color, 0) = static_cast<float>(red);
    CSF_AT(float, color, 4) = static_cast<float>(green);
    CSF_AT(float, color, 8) = static_cast<float>(blue);
    CSF_AT(float, color, 12) = 255.0f;
}

static void MenuMpServerInfo_SetStatisticHeaders(
    uint32_t& listAddress, void* color, int team)
{
    for (int column = 2; column <= 5; ++column) {
        MenuMpServerInfo_ReadHeaderColor(color, team);
        int key;
        switch (column) {
        case 2: key = 1937125681; break;
        case 3: key = 1937125432; break;
        case 4: key = 1937125433; break;
        default: key = 1937125680; break;
        }
        MenuMpServerInfo_SetLocalizedHeader(
            column, reinterpret_cast<uint32_t*>(color),
            static_cast<int>(listAddress), key, 1);
    }
}

static void MenuMpServerInfo_FormatTeamCount(
    void* output, wchar_t* text, uint16_t count)
{
    alignas(8) uint8_t temporaryBytes[0x20];
    ConstructWideString(temporaryBytes, nullptr, text);

    MenuMpServerInfo_WideStringArgument format;
    std::memcpy(&format, temporaryBytes, sizeof(format));

    // The supplied one-argument binding describes only the fixed prefix.
    // The original also passes the owning string value and the count.
    using FormatWithArguments =
        void*(__cdecl*)(void*, MenuMpServerInfo_WideStringArgument, ...);
    reinterpret_cast<FormatWithArguments>(
        CSF_ADDR_FN(0x00696A80))(output, format, static_cast<int>(count));
}

static void* MenuMpServerInfo_SetSpectatorHeaders(
    CMenuMpServerInfo* self, void* color)
{
    CSF_AT(float, color, 0) = 125.0f;
    CSF_AT(float, color, 4) = 132.0f;
    CSF_AT(float, color, 8) = 102.0f;
    CSF_AT(float, color, 12) = 255.0f;
    MenuMpServerInfo_SetLocalizedHeader(
        1, reinterpret_cast<uint32_t*>(color),
        static_cast<int>(self->field_001c), 1937125940, 0);

    CSF_AT(float, color, 0) = 125.0f;
    CSF_AT(float, color, 4) = 132.0f;
    CSF_AT(float, color, 8) = 102.0f;
    CSF_AT(float, color, 12) = 255.0f;
    return MenuMpServerInfo_SetLocalizedHeader(
        5, reinterpret_cast<uint32_t*>(color),
        static_cast<int>(self->field_001c), 1937125680, 1);
}

static void* __fastcall CMenuMpServerInfo_InitializeStatisticHeaders(
    CMenuMpServerInfo* self, void* /*edx*/)
{
    MenuMpServerInfo_ClearList(self->field_001c);
    MenuMpServerInfo_ClearList(self->field_0010);
    MenuMpServerInfo_ClearList(self->field_0014);
    MenuMpServerInfo_ClearList(self->field_0018);
    MenuMpServerInfo_UpdateItemHeight(self->field_001c);

    alignas(8) uint8_t colorBytes[0x14];
    uint32_t mode = self->field_01e4;

    if (mode == 0) {
        CGuiListBoxBase_EnsureColumn(
            reinterpret_cast<CGuiListBoxBase*>(self->field_0010), nullptr, 6);
        CGuiListBoxBase_EnsureColumn(
            reinterpret_cast<CGuiListBoxBase*>(self->field_001c), nullptr, 6);
        MenuMpServerInfo_UpdateItemHeight(self->field_0010);
        MenuMpServerInfo_SetColumnWidths(self->field_0010);
        MenuMpServerInfo_SetColumnWidths(self->field_001c);

        MenuMpServerInfo_ReadHeaderColor(colorBytes, 0);
        MenuMpServerInfo_SetLocalizedHeader(
            1, reinterpret_cast<uint32_t*>(colorBytes),
            static_cast<int>(self->field_0010), 1937125937, 0);
        MenuMpServerInfo_SetStatisticHeaders(self->field_0010, colorBytes, 0);
        return MenuMpServerInfo_SetSpectatorHeaders(self, colorBytes);
    }

    if (mode != 1 && mode != 3)
        return reinterpret_cast<void*>(mode - 3);

    CGuiListBoxBase_EnsureColumn(
        reinterpret_cast<CGuiListBoxBase*>(self->field_0014), nullptr, 6);
    CGuiListBoxBase_EnsureColumn(
        reinterpret_cast<CGuiListBoxBase*>(self->field_0018), nullptr, 6);
    CGuiListBoxBase_EnsureColumn(
        reinterpret_cast<CGuiListBoxBase*>(self->field_001c), nullptr, 6);

    MenuMpServerInfo_UpdateItemHeight(self->field_0014);
    MenuMpServerInfo_SetColumnWidths(self->field_0014);
    MenuMpServerInfo_UpdateItemHeight(self->field_0018);
    MenuMpServerInfo_SetColumnWidths(self->field_0018);
    MenuMpServerInfo_UpdateItemHeight(self->field_001c);
    MenuMpServerInfo_SetColumnWidths(self->field_001c);

    void* teamState = reinterpret_cast<void*>(GetGlobal8B0688() + 3240);
    alignas(8) uint8_t stringBytes[0x20];
    CSF_AT(uint32_t, stringBytes, 0) = 0;
    CSF_AT(uint16_t, stringBytes, 4) = 0;
    CSF_AT(uint32_t, stringBytes, 20) = 0;
    CSF_AT(uint32_t, stringBytes, 24) = 7;

    alignas(8) uint8_t redTextBytes[0x64];
    int entry = MenuMessageBox_ResolveEntryByKey(1937125944, 2);
    MenuLoading_CopyLocalizedText(
        reinterpret_cast<uint16_t*>(redTextBytes),
        reinterpret_cast<const uint16_t*>(entry), 50);
    uint16_t redCount = CSF_AT(uint16_t, teamState, 1792);
    MenuMpServerInfo_FormatTeamCount(
        stringBytes, reinterpret_cast<wchar_t*>(redTextBytes), redCount);
    MenuMpServerInfo_ReadHeaderColor(colorBytes, 1);
    MenuMpServerInfo_SetStringHeader(
        1, reinterpret_cast<uint32_t*>(colorBytes),
        static_cast<int>(self->field_0014), stringBytes, 0);
    MenuMpServerInfo_SetStatisticHeaders(self->field_0014, colorBytes, 1);

    alignas(8) uint8_t blueTextBytes[0x64];
    entry = MenuMessageBox_ResolveEntryByKey(1937125945, 2);
    MenuLoading_CopyLocalizedText(
        reinterpret_cast<uint16_t*>(blueTextBytes),
        reinterpret_cast<const uint16_t*>(entry), 50);
    uint16_t blueCount = CSF_AT(uint16_t, teamState, 1794);
    MenuMpServerInfo_FormatTeamCount(
        stringBytes, reinterpret_cast<wchar_t*>(blueTextBytes), blueCount);
    MenuMpServerInfo_ReadHeaderColor(colorBytes, 2);
    MenuMpServerInfo_SetStringHeader(
        1, reinterpret_cast<uint32_t*>(colorBytes),
        static_cast<int>(self->field_0018), stringBytes, 0);
    MenuMpServerInfo_SetStatisticHeaders(self->field_0018, colorBytes, 2);

    void* result = MenuMpServerInfo_SetSpectatorHeaders(self, colorBytes);
    if (CSF_AT(uint32_t, stringBytes, 24) >= 8) {
        result = CSF_AT(void*, stringBytes, 4);
        if (result)
            return reinterpret_cast<void*>(Mem_FreeDebug(result));
    }
    return result;
}

HOOK(0x00531440, CMenuMpServerInfo_InitializeStatisticHeaders, "auto");
