#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Callees
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());

// Data
CSF_GLOBAL(const char*, g_pszMenu_P_Tactic, 0x00899B14);
CSF_GLOBAL(const char*, g_pszMenu_P_Help, 0x00899AA4);
CSF_GLOBAL(const char*, g_pszMenu_P_MeJuMP, 0x00899AF8);
CSF_GLOBAL(const char*, g_pszMenu_P_MeJueg, 0x00899AF4);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// Virtual slot signatures on the menu object (vtable offsets 0x58, 0x5C, 0x74)
using MenuFindItemFn = int(__fastcall*)(void*, void*, const char*, int);
using MenuItemFlagFn = bool(__fastcall*)(void*, void*, int);
using MenuSetTextFn = int(__fastcall*)(void*, void*, const char*);

static char __fastcall Interfaz_RefreshPendingMenuState(void* self, void* /*edx*/)
{
    char result = static_cast<char>(CSF_AT(uint8_t, self, 0x182));
    if (result)
    {
        void* menu = CSF_AT(void*, self, 0x18);
        void** vt = CSF_AT(void**, menu, 0);
        int tacticItem = reinterpret_cast<MenuFindItemFn>(vt[23])(
            menu, nullptr, g_pszMenu_P_Tactic, 1);
        result = static_cast<char>(reinterpret_cast<MenuItemFlagFn>(vt[29])(
            CSF_AT(void*, self, 0x18), nullptr, tacticItem));
        if (!result)
        {
            menu = CSF_AT(void*, self, 0x18);
            vt = CSF_AT(void**, menu, 0);
            int helpItem = reinterpret_cast<MenuFindItemFn>(vt[23])(
                menu, nullptr, g_pszMenu_P_Help, 1);
            result = static_cast<char>(reinterpret_cast<MenuItemFlagFn>(vt[29])(
                CSF_AT(void*, self, 0x18), nullptr, helpItem));
            if (!result)
            {
                void* scene = g_pCurrentScene;
                void* sceneSub = CSF_AT(void*, scene, 0xC0);
                result = static_cast<char>(CSF_AT(uint8_t, sceneSub, 9));
                if (!result)
                {
                    uint8_t* globalState = reinterpret_cast<uint8_t*>(
                        static_cast<uintptr_t>(GetGlobalState_50D190()));
                    uint8_t stateFlag = CSF_AT(uint8_t, globalState, 0xD0);
                    const char* text = g_pszMenu_P_MeJueg;
                    if (stateFlag)
                        text = g_pszMenu_P_MeJuMP;
                    menu = CSF_AT(void*, self, 0x18);
                    vt = CSF_AT(void**, menu, 0);
                    result = static_cast<char>(reinterpret_cast<MenuSetTextFn>(vt[22])(
                        menu, nullptr, text));
                }
            }
        }
        CSF_AT(uint8_t, self, 0x182) = 0;
    }
    return result;
}

HOOK(0x00535ED0, Interfaz_RefreshPendingMenuState, "auto");
