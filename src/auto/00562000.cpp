#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, MpServerInfo_InitDefaultStyle, 0x00530AB0, (uint32_t* self, void*));
CSF_FUNC(int, __cdecl, GetGlobal8B0688, 0x00550CC0, ());
CSF_FUNC(void*, __fastcall, LookupObjectById, 0x00652160, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, MenuMessageBox_ResolveEntryByKey, 0x00692440, (int code, int flag));
CSF_FUNC(std::uint32_t*, __fastcall, MenuProgressInput_AssignText, 0x00696F90, (std::uint32_t* self, void* edx, std::int16_t* text));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, CGuiListBoxBase_SetItemHeight, 0x00714750, (CGuiListBoxBase* self, void* , int itemHeight));
CSF_FUNC(int, __fastcall, FreeAndResetBuffer, 0x007158E0, (int* self, void*));
CSF_FUNC(void, __fastcall, CGuiListBoxBase_SetColumnWidth, 0x00716520, (CGuiListBoxBase* self, void* , std::uint32_t columnIndex, std::int32_t width));
CSF_FUNC(int, __fastcall, CGuiTextureListBox_ExtendToRowIndex, 0x00717970, (CGuiTextureListBox* self, void* , int rowIndex));
CSF_FUNC(void*, __fastcall, ForwardToStoredObjectVirtualSlot40, 0x00722DF0, (void* self, void* edx));

CSF_GLOBAL(float, g_QuarterTurnConstant, 0x0082FAD4);
CSF_GLOBAL(float, g_MinWidePitch, 0x00833E38);
CSF_GLOBAL(float, g_MaxWidePitch, 0x00833E3C);
CSF_GLOBAL(float, g_MenuWidthScale, 0x0083F24C);
CSF_GLOBAL(float, g_MenuHeightScale, 0x0083F250);
CSF_GLOBAL(float, g_SessionNegativeMapWidth, 0x008459F4);
CSF_GLOBAL(float, g_SessionNegativeNameWidth, 0x008459F8);
CSF_GLOBAL(float, g_SessionMapWidth, 0x008459FC);
CSF_GLOBAL(float, g_SessionNameWidth, 0x00845A00);
CSF_GLOBAL(int, g_MpNetworkType, 0x0089A5F0);
CSF_GLOBAL(std::uint8_t, g_OnlineMatchFlag, 0x008B068C);

static void* MenuMpSessions_GetScreenDimensions()
{
    void* object = LookupObjectById(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    return reinterpret_cast<void* (__fastcall*)(void*, void*)>(
        CSF_AT(void**, object, 0)[0x18 / 4])(object, nullptr);
}

static std::int32_t MenuMpSessions_GetScaledColumnWidth(const float& width)
{
    void* dimensions = MenuMpSessions_GetScreenDimensions();
    return static_cast<std::int32_t>(static_cast<std::int64_t>(
        static_cast<float>(CSF_AT(int, dimensions, 0)) *
        g_MenuWidthScale * width));
}

static void MenuMpSessions_SetColumnHeader(
    CMenuMpSessions* self, std::uint8_t* style,
    std::uint32_t column, int messageKey)
{
    auto* text = reinterpret_cast<std::int16_t*>(
        MenuMessageBox_ResolveEntryByKey(messageKey, 1));
    MenuProgressInput_AssignText(
        reinterpret_cast<std::uint32_t*>(style + 0x18), nullptr, text);

    void* list = reinterpret_cast<void*>(self->field_0070);
    reinterpret_cast<void (__fastcall*)(
        void*, void*, std::uint32_t, void*, const char*)>(
        CSF_AT(void**, list, 0)[0x258 / 4])(
            list, nullptr, column, style,
            CSF_STR(0x0082FFA4, "Menus/Ingame_01.fnt"));
}

static std::int32_t MenuMpSessions_SubtractWidth(
    std::int32_t extraWidth, std::int32_t negativeWidth)
{
    return static_cast<std::int32_t>(
        static_cast<std::uint32_t>(extraWidth) -
        static_cast<std::uint32_t>(negativeWidth));
}

static void __fastcall CMenuMpSessions_InitializeSessionList(
    CMenuMpSessions* self, void* /*edx*/)
{
    if (!self->field_0070)
        return;

    FreeAndResetBuffer(reinterpret_cast<int*>(self->field_0070), nullptr);
    CGuiTextureListBox_ExtendToRowIndex(
        reinterpret_cast<CGuiTextureListBox*>(self->field_0070), nullptr, 10);

    void* content = ForwardToStoredObjectVirtualSlot40(
        reinterpret_cast<void*>(self->field_0070), nullptr);
    if (!content)
        return;

    int itemHeight = reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, content, 0)[0x18 / 4])(content, nullptr, 1);
    CGuiListBoxBase_SetItemHeight(
        reinterpret_cast<CGuiListBoxBase*>(self->field_0070),
        nullptr, itemHeight);

    // The style, its text member, and trailing layout values share one
    // contiguous stack record. The initializer writes into both portions.
    alignas(8) std::uint8_t styleBytes[0x4C];
    MpServerInfo_InitDefaultStyle(
        reinterpret_cast<std::uint32_t*>(styleBytes), nullptr);

    CSF_AT(std::uint32_t, styleBytes, 0x3C) = 1130561536;
    CSF_AT(std::uint32_t, styleBytes, 0x08) = 1130561536;
    CSF_AT(std::uint32_t, styleBytes, 0x40) = 1125842944;
    CSF_AT(std::uint32_t, styleBytes, 0x44) = 1116340224;
    CSF_AT(std::uint32_t, styleBytes, 0x48) = 1132396544;
    CSF_AT(std::uint32_t, styleBytes, 0x0C) = 1125842944;
    CSF_AT(std::uint32_t, styleBytes, 0x10) = 1116340224;
    CSF_AT(std::uint32_t, styleBytes, 0x14) = 1132396544;

    void* dimensions = MenuMpSessions_GetScreenDimensions();
    CSF_AT(float, styleBytes, 0x38) =
        static_cast<float>(CSF_AT(int, dimensions, 0x04)) * g_MenuHeightScale;
    dimensions = MenuMpSessions_GetScreenDimensions();
    CSF_AT(float, styleBytes, 0x34) =
        static_cast<float>(CSF_AT(int, dimensions, 0)) * g_MenuWidthScale;

    int online;
    if (GetGlobal8B0688())
    {
        GetGlobal8B0688();
        online = g_OnlineMatchFlag;
    }
    else
    {
        online = g_MpNetworkType == 3;
    }

    if (online)
    {
        MenuMpSessions_SetColumnHeader(self, styleBytes, 0, 1297100848);
        auto width = MenuMpSessions_GetScaledColumnWidth(g_SessionNameWidth);
        CGuiListBoxBase_SetColumnWidth(
            reinterpret_cast<CGuiListBoxBase*>(self->field_0070),
            nullptr, 0, width);

        MenuMpSessions_SetColumnHeader(self, styleBytes, 1, 1297100849);
        width = MenuMpSessions_GetScaledColumnWidth(g_MaxWidePitch);
        CGuiListBoxBase_SetColumnWidth(
            reinterpret_cast<CGuiListBoxBase*>(self->field_0070),
            nullptr, 1, width);

        MenuMpSessions_SetColumnHeader(self, styleBytes, 2, 1297100850);
        width = MenuMpSessions_GetScaledColumnWidth(g_SessionMapWidth);
        CGuiListBoxBase_SetColumnWidth(
            reinterpret_cast<CGuiListBoxBase*>(self->field_0070),
            nullptr, 2, width);

        MenuMpSessions_SetColumnHeader(self, styleBytes, 3, 1297100851);
        width = MenuMpSessions_GetScaledColumnWidth(g_QuarterTurnConstant);
        CGuiListBoxBase_SetColumnWidth(
            reinterpret_cast<CGuiListBoxBase*>(self->field_0070),
            nullptr, 3, width);
    }
    else
    {
        const std::int32_t extraWidth =
            MenuMpSessions_GetScaledColumnWidth(g_QuarterTurnConstant) / 3;

        MenuMpSessions_SetColumnHeader(self, styleBytes, 0, 1297100848);
        auto width = MenuMpSessions_GetScaledColumnWidth(g_SessionNegativeNameWidth);
        CGuiListBoxBase_SetColumnWidth(
            reinterpret_cast<CGuiListBoxBase*>(self->field_0070),
            nullptr, 0, MenuMpSessions_SubtractWidth(extraWidth, width));

        MenuMpSessions_SetColumnHeader(self, styleBytes, 1, 1297100849);
        width = MenuMpSessions_GetScaledColumnWidth(g_MinWidePitch);
        CGuiListBoxBase_SetColumnWidth(
            reinterpret_cast<CGuiListBoxBase*>(self->field_0070),
            nullptr, 1, MenuMpSessions_SubtractWidth(extraWidth, width));

        MenuMpSessions_SetColumnHeader(self, styleBytes, 2, 1297100850);
        width = MenuMpSessions_GetScaledColumnWidth(g_SessionNegativeMapWidth);
        CGuiListBoxBase_SetColumnWidth(
            reinterpret_cast<CGuiListBoxBase*>(self->field_0070),
            nullptr, 2, MenuMpSessions_SubtractWidth(extraWidth, width));
    }

    if (CSF_AT(std::uint32_t, styleBytes, 0x30) >= 8)
    {
        void* block = CSF_AT(void*, styleBytes, 0x1C);
        if (block)
            Mem_FreeDebug(block);
    }
}

HOOK(0x562000, CMenuMpSessions_InitializeSessionList, "auto");
