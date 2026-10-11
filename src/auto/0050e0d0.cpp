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

CSF_FUNC(int, __cdecl, GetGlobal8afc7c, 0x0043E010, ());
CSF_FUNC(int, __fastcall, sub_43E6C0, 0x0043E6C0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(int, __fastcall, CSoundManager_ResetThirdManager, 0x00523640, (void* self, void* edx));

CSF_GLOBAL(const char*, g_pszMenu_P_MeJueg, 0x00899AF4);

// Resets the sound manager, then activates the game menu screen (if not
// suppressed by field_00d0) and tail-jumps into the menu panel's
// virtual slot 0x24.
static hook::Tail __fastcall CMenuManager_ResetSoundAndActivateGameMenu(void* selfRaw, const uint32_t* /*args*/)
{
    CMenuManager* self = reinterpret_cast<CMenuManager*>(selfRaw);

    int globalObjRaw = GetGlobal8afc7c();
    void* globalObj = reinterpret_cast<void*>(static_cast<uintptr_t>(globalObjRaw));
    sub_43E6C0(globalObj, nullptr);

    self->vSlot84();

    int soundRaw = GetGlobalValue_521BB0();
    void* soundMgr = reinterpret_cast<void*>(static_cast<uintptr_t>(soundRaw));
    CSoundManager_ResetThirdManager(soundMgr, nullptr);

    if (!self->field_00d0) {
        self->vSlot2C(static_cast<int>(self->m_pScreenGameMenu),
                      static_cast<int>(reinterpret_cast<uintptr_t>(g_pszMenu_P_MeJueg)));
    }

    void* panel = reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_007c));
    return hook::tail_jump(panel, (*reinterpret_cast<void***>(panel))[0x24 / 4]);
}

HOOK_TAIL(0x0050E0D0, CMenuManager_ResetSoundAndActivateGameMenu, "game_bdd_loader");
