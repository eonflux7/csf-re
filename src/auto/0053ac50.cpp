#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Menu sound/string name forwarded to the sound manager.
CSF_GLOBAL(const char*, g_pszMenu_P_Sound, 0x899AC4);

// Member object at this+0x14 exposes a virtual "play/notify" method at vtable slot 22 (0x58).
// Original: load soundManager := [this+0x14], push g_pszMenu_P_Sound, call [vtbl+0x58].
static int __fastcall CMenuGameOptions_PlayMenuSound(void* self, void* /*edx*/)
{
    void* soundManager = CSF_AT(void*, self, 0x14);
    void** vtbl = *reinterpret_cast<void***>(soundManager);
    return ((int(__fastcall*)(void*, void*, const char*))vtbl[0x58 / 4])(
        soundManager, nullptr, g_pszMenu_P_Sound);
}

HOOK(0x53AC50, CMenuGameOptions_PlayMenuSound, "auto");
