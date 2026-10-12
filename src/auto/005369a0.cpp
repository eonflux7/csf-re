#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_8B0568, 0x004FDBB0, ());
CSF_FUNC(void*, __fastcall, UmsVirtualProcessor_GetDefaultDestination, 0x004FE4A0, (void* self, void*));
CSF_FUNC(int, __fastcall, CMenuPause_CloseAndReturnResult, 0x00539D10, (CMenuPause* self, void* , int a2));
CSF_FUNC(int, __fastcall, CMenuMissionInfo_EnterPanelState, 0x0053D740, (CMenuMissionInfo* self, void* , int));

CSF_GLOBAL(const char*, g_pszMenu_P_Help, 0x00899AA4);
CSF_GLOBAL(const char*, g_pszMenu_P_MeJueg, 0x00899AF4);
CSF_GLOBAL(const char*, g_pszMenu_P_Tactic, 0x00899B14);

static char __fastcall Interface_RestorePendingHelpPanel(void* self, void* /*edx*/)
{
    char result = CSF_AT(char, self, 0x188);
    if (!result)
        return result;

    void* menuManager = CSF_AT(void*, self, 0x18);
    void* pauseMenu =
        ((void* (__fastcall*)(void*, void*, const char*))
            CSF_AT(void**, menuManager, 0)[0x5C / 4])(
                menuManager, nullptr, g_pszMenu_P_MeJueg);

    menuManager = CSF_AT(void*, self, 0x18);
    bool shouldRemove =
        ((bool (__fastcall*)(void*, void*, void*))
            CSF_AT(void**, menuManager, 0)[0x78 / 4])(
                menuManager, nullptr, pauseMenu);

    menuManager = CSF_AT(void*, self, 0x18);
    if (shouldRemove)
    {
        ((void (__fastcall*)(void*, void*, void*, int))
            CSF_AT(void**, menuManager, 0)[0x60 / 4])(
                menuManager, nullptr, pauseMenu, 0);
    }
    else if (((bool (__fastcall*)(void*, void*, void*, int))
                  CSF_AT(void**, menuManager, 0)[0x74 / 4])(
                      menuManager, nullptr, pauseMenu, 0))
    {
        CMenuPause_CloseAndReturnResult(
            static_cast<CMenuPause*>(pauseMenu), nullptr, 1);
    }

    void* globalObject = reinterpret_cast<void*>(GetGlobalValue_8B0568());
    void* destination =
        UmsVirtualProcessor_GetDefaultDestination(globalObject, nullptr);

    if (CSF_AT(std::uint32_t, destination, 0x1C) >= 2u)
    {
        menuManager = CSF_AT(void*, self, 0x18);
        void* tacticalMenu =
            ((void* (__fastcall*)(void*, void*, const char*))
                CSF_AT(void**, menuManager, 0)[0x5C / 4])(
                    menuManager, nullptr, g_pszMenu_P_Tactic);

        menuManager = CSF_AT(void*, self, 0x18);
        shouldRemove =
            ((bool (__fastcall*)(void*, void*, void*))
                CSF_AT(void**, menuManager, 0)[0x78 / 4])(
                    menuManager, nullptr, tacticalMenu);

        menuManager = CSF_AT(void*, self, 0x18);
        if (shouldRemove)
        {
            ((void (__fastcall*)(void*, void*, void*, int))
                CSF_AT(void**, menuManager, 0)[0x60 / 4])(
                    menuManager, nullptr, tacticalMenu, 0);
        }
        else if (((bool (__fastcall*)(void*, void*, void*, int))
                      CSF_AT(void**, menuManager, 0)[0x74 / 4])(
                          menuManager, nullptr, tacticalMenu, 0))
        {
            CMenuMissionInfo_EnterPanelState(
                static_cast<CMenuMissionInfo*>(tacticalMenu), nullptr, 1);
        }
    }

    menuManager = CSF_AT(void*, self, 0x18);
    void* helpMenu =
        ((void* (__fastcall*)(void*, void*, const char*))
            CSF_AT(void**, menuManager, 0)[0x5C / 4])(
                menuManager, nullptr, g_pszMenu_P_Help);

    std::memcpy(
        static_cast<std::uint8_t*>(helpMenu) + 0x44,
        static_cast<std::uint8_t*>(self) + 0x18C,
        0xE4u);

    menuManager = CSF_AT(void*, self, 0x18);
    result =
        ((char (__fastcall*)(void*, void*, const char*))
            CSF_AT(void**, menuManager, 0)[0x58 / 4])(
                menuManager, nullptr, g_pszMenu_P_Help);

    CSF_AT(char, self, 0x188) = 0;
    return result;
}

HOOK(0x005369A0, Interface_RestorePendingHelpPanel, "auto");
