#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(const char*, g_pszMenu_P_Juego, 0x00899AF0);
CSF_GLOBAL(int, g_MpNetworkType, 0x0089A5F0);
CSF_GLOBAL(std::uint8_t, g_OnlineMatchFlag, 0x008B068C);

CSF_FUNC(int, __cdecl, GetGlobal8B0688, 0x00550CC0, ());
CSF_FUNC(void, __cdecl, MenuAccounts_ResetPendingEdit, 0x006A6D40, ());
CSF_FUNC(int, __cdecl, GetGlobalDword_73C020, 0x0073C020, ());
CSF_FUNC(int, __fastcall, MenuCharSelect_FinalizePreviewSlot, 0x0073E920, (int* self, void* edx));

static int __fastcall CMenuMpOnline_ActivateOnlineMenu(void* self, void* /*edx*/)
{
    // Step 1: notify the child menu object stored at +0x3C with (this, 0).
    void** menuObj = CSF_AT(void**, self, 0x3C);
    void** vtbl = CSF_AT(void**, menuObj, 0);
    reinterpret_cast<void(__fastcall*)(void*, void*, void*, int)>(vtbl[24])(menuObj, nullptr, self, 0);

    // Step 2: look up the handle for g_pszMenu_P_Juego (thiscall on the child menu, vtable slot 23).
    menuObj = CSF_AT(void**, self, 0x3C);
    void** vtblForCalls = CSF_AT(void**, menuObj, 0);
    int stringHandle = reinterpret_cast<int(__fastcall*)(void*, void*, const char*, int)>(vtblForCalls[23])(menuObj, nullptr, g_pszMenu_P_Juego, 0);

    // Step 3: query the child menu with the handle (thiscall, vtable slot 29).
    menuObj = CSF_AT(void**, self, 0x3C);
    int result = reinterpret_cast<int(__fastcall*)(void*, void*, int)>(vtblForCalls[29])(menuObj, nullptr, stringHandle);
    if (static_cast<std::uint8_t>(result) != 0)
        return result;

    int onlineActive;
    if (GetGlobal8B0688() != 0)
    {
        GetGlobal8B0688();
        onlineActive = g_OnlineMatchFlag;
    }
    else
    {
        onlineActive = (g_MpNetworkType == 3);
    }

    if (onlineActive)
        MenuAccounts_ResetPendingEdit();

    int sessionHandle = GetGlobalDword_73C020();
    return MenuCharSelect_FinalizePreviewSlot(reinterpret_cast<int*>(sessionHandle), nullptr);
}

HOOK(0x0055F720, CMenuMpOnline_ActivateOnlineMenu, "auto");
