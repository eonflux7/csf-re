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

CSF_FUNC(int, __fastcall, SceneLoader_ClearAdditionalResources, 0x0044C540, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_8B0568, 0x004FDBB0, ());
CSF_FUNC(void*, __fastcall, UmsVirtualProcessor_GetDefaultDestination, 0x004FE4A0, (void* self, void*));

CSF_GLOBAL(const char*, g_pszMenu_P_Tactic, 0x00899B14);

// Handles the "menu P_Tactic" scene-instancing event (event id 14). When the
// event's payload matches the tactic menu's current id, marks the state as 2
// and clears the scene's additional resources.
static void __fastcall C_00843720_OnTacticMenuEvent(C_00843720* self, void* /*edx*/, int* a2)
{
    if (a2[0] != 14)
        return;

    uint32_t menuHolder = CSF_AT(uint32_t, self, 0x18);
    if (!menuHolder)
        return;

    // Virtual slot 0x5c / 4 = 23: lookup of the named menu by string.
    void* holderObj = reinterpret_cast<void*>(menuHolder);
    void* menu = ((void* (__fastcall*)(void*, void*, const char*))CSF_AT(void**, holderObj, 0)[0x5c / 4])(
        holderObj, nullptr, g_pszMenu_P_Tactic);
    if (!menu)
        return;

    // Virtual slot 0x20 / 4 = 8: get the menu's id.
    int menuId = ((int (__fastcall*)(void*, void*))CSF_AT(void**, menu, 0)[0x20 / 4])(menu, nullptr);
    if (menuId != a2[1])
        return;

    CSF_AT(int, self, 0x130) = 2;

    int globalValue = GetGlobalValue_8B0568();
    void* defaultDestination = UmsVirtualProcessor_GetDefaultDestination(
        reinterpret_cast<void*>(static_cast<uintptr_t>(globalValue)), nullptr);
    SceneLoader_ClearAdditionalResources(defaultDestination, nullptr);
}

HOOK(0x535E70, C_00843720_OnTacticMenuEvent, "auto");
