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

// Returns the state block for the current user/session. Byte at +0xCC selects the panel.
CSF_FUNC(int, __cdecl, GetGlobalValue_75FDF0, 0x0075FDF0, ());

CSF_GLOBAL(const char*, g_pszMenu_P_Profil, 0x00899AE0);
CSF_GLOBAL(const char*, g_pszMenu_P_MePrin, 0x00899AE4);

// Slot 0x58 of the panel object: __stdcall taking one string argument.
using PanelSlot22Fn = int(__stdcall*)(const char* panelName);

// Shows the initial menu panel: "P_Profil" when the session flag at +0xCC is set,
// otherwise "P_MePrin". The name is forwarded to the panel manager at field_0014.
static int __fastcall CMenuManager_ShowInitialPanel(CMenuManager* self, void* /*edx*/)
{
    int globalValueAddr = GetGlobalValue_75FDF0();
    uint8_t* globalValue = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(globalValueAddr));

    const char* panelName = g_pszMenu_P_Profil;
    if (!CSF_AT(uint8_t, globalValue, 0xCC))
        panelName = g_pszMenu_P_MePrin;

    void* panelObj = reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0014));
    PanelSlot22Fn slotFn = reinterpret_cast<PanelSlot22Fn>(CSF_AT(void**, panelObj, 0)[0x58 / 4]);
    return slotFn(panelName);
}

HOOK(0x0050E300, CMenuManager_ShowInitialPanel, "auto");
