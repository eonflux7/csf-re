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

// Globals holding the name strings used to look up child panels.
CSF_GLOBAL(const char*, g_pszMenu_P_Juego, 0x899AF0);
CSF_GLOBAL(const char*, g_pszMenu_P_TmPart, 0x899B20);

// Resets the pause menu panel state (__thiscall, one stack arg, ret 4).
CSF_FUNC(void, __fastcall, CMenuMpPause_OnActivate, 0x534AE0, (CMenuMpPause* self, void* edx, int a2));

// Virtual slot 0x5c: find a child panel by name. Returns the child or null.
static void* FindChildPanelByName(void* panel, const char* name)
{
    void** vtbl = *(void***)panel;
    auto fn = (void*(__fastcall*)(void*, void*, const char*))vtbl[0x5c / 4];
    return fn(panel, nullptr, name);
}

// Virtual slot 0x54 on a child panel: set its state flag to 1.
static void SetChildPanelFlagOn(void* child)
{
    void** vtbl = *(void***)child;
    auto fn = (void(__fastcall*)(void*, void*, int))vtbl[0x54 / 4];
    fn(child, nullptr, 1);
}

// Virtual slot 0x60 on the parent panel: apply the state to the child with arg 0.
static void SetPanelChildState(void* panel, void* child)
{
    void** vtbl = *(void***)panel;
    auto fn = (void(__fastcall*)(void*, void*, void*, int))vtbl[0x60 / 4];
    fn(panel, nullptr, child, 0);
}

// Initializes the multiplayer pause menu panels and marks the menu as initialized.
static void __fastcall CMenuMpPause_InitPauseMenuPanels(CMenuMpPause* self, void* /*edx*/)
{
    // "P_Juego" child: set its flag on.
    void* juegoPanel = FindChildPanelByName(reinterpret_cast<void*>(self->field_0010), g_pszMenu_P_Juego);
    if (juegoPanel)
        SetChildPanelFlagOn(juegoPanel);

    // "P_TmPart" child: apply state with flag 0 through the parent panel.
    void* tmPartPanel = FindChildPanelByName(reinterpret_cast<void*>(self->field_0010), g_pszMenu_P_TmPart);
    if (tmPartPanel)
        SetPanelChildState(reinterpret_cast<void*>(self->field_0010), tmPartPanel);

    self->field_0044 = 1;
    CMenuMpPause_OnActivate(self, nullptr, 0);
}

HOOK(0x534E40, CMenuMpPause_InitPauseMenuPanels, "auto");
