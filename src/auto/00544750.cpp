#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Localized title string pointer used by the save-game menu.
CSF_GLOBAL(const char*, g_pszMenu_P_MeJueg, 0x899AF4);

// menu_save_game : CMenuSaveGame
//
// If the menu's state field (offset 0x50) is still -1, mark it as 6, notify the
// owning menu object (virtual slot at +0x60 with (this, 0)), and then let the
// owning menu apply the localized title string (virtual slot at +0x58).
static int __fastcall CMenuSaveGame_ActivateWithTitle(void* self, void* /*edx*/)
{
    if (CSF_AT(int32_t, self, 0x50) == -1)
    {
        void* pOwner = CSF_AT(void*, self, 0x40);
        CSF_AT(int32_t, self, 0x50) = 6;

        void** pOwnerVtbl = reinterpret_cast<void**>(CSF_AT(void*, pOwner, 0));
        ((void(__fastcall*)(void*, void*, void*, int))pOwnerVtbl[0x60 / 4])(pOwner, nullptr, self, 0);

        // The first virtual call may replace the owner object, so reload it from self.
        void* pOwnerAfter = CSF_AT(void*, self, 0x40);
        void** pOwnerVtblAfter = reinterpret_cast<void**>(CSF_AT(void*, pOwnerAfter, 0));
        return ((int(__fastcall*)(void*, void*, const char*))pOwnerVtblAfter[0x58 / 4])(
            pOwnerAfter, nullptr, g_pszMenu_P_MeJueg);
    }

    // Original falls through without touching eax; return 0 for determinism.
    return 0;
}

HOOK(0x544750, CMenuSaveGame_ActivateWithTitle, "auto");
