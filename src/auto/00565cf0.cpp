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

// Global pointer to the menu-name string used as the lookup key for the menu object.
CSF_GLOBAL(const char*, g_pszMenu_P_LbQMch, 0x00899B48);

// Applies the deathmatch selection: asks the owner to refresh its state, looks up the
// menu object named by g_pszMenu_P_LbQMch, and writes the mode selector at +0x20.
// a2 == 0 -> 0, a2 == 1 -> 1, a2 == 2 -> 3; any other value leaves the menu untouched.
static int __fastcall CMenuMpGameMode_ApplyDeathmatchSelection(CMenuMpGameMode* self, void* /*edx*/, int a2)
{
    // First virtual call (slot 0x60 / 4 = 24): owner->Refresh(self, 0)
    void* owner = reinterpret_cast<void*>(self->field_0014);
    ((void(__fastcall*)(void*, void*, void*, int))(CSF_AT(void**, owner, 0)[0x60 / 4]))(
        owner, nullptr, self, 0);

    // Second virtual call (slot 0x58 / 4 = 22): menu = owner->FindMenu(g_pszMenu_P_LbQMch)
    owner = reinterpret_cast<void*>(self->field_0014);
    void* menu = ((void*(__fastcall*)(void*, void*, const char*))(CSF_AT(void**, owner, 0)[0x58 / 4]))(
        owner, nullptr, g_pszMenu_P_LbQMch);

    if (a2 != 0) {
        if (a2 == 1) {
            CSF_AT(uint32_t, menu, 0x20) = 1;
        } else if (a2 == 2) {
            CSF_AT(uint32_t, menu, 0x20) = 3;
        }
    } else {
        CSF_AT(uint32_t, menu, 0x20) = 0;
    }

    return reinterpret_cast<int>(menu);
}

HOOK(0x00565CF0, CMenuMpGameMode_ApplyDeathmatchSelection, "auto");
