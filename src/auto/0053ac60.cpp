#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Menu string name forwarded to the menu/sound manager (video menu entry).
CSF_GLOBAL(const char*, g_pszMenu_P_Video, 0x899AB0);

// Member object at this+0x14 exposes a virtual method at vtable slot 22 (0x58) that
// takes a string name. Original: ecx := [this+0x14]; push g_pszMenu_P_Video;
// eax := [ecx]; call [eax+0x58].
static int __fastcall CMenuGameOptions_PlayMenuVideo(void* self, void* /*edx*/)
{
    void* menuManager = CSF_AT(void*, self, 0x14);
    void** vtbl = *reinterpret_cast<void***>(menuManager);
    return ((int(__fastcall*)(void*, void*, const char*))vtbl[0x58 / 4])(
        menuManager, nullptr, g_pszMenu_P_Video);
}

HOOK(0x53AC60, CMenuGameOptions_PlayMenuVideo, "auto");
