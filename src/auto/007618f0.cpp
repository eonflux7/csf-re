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

// Table of 0x18 const char* entries (multiplayer animation type names).
CSF_GLOBAL(const char*, g_psz_ESTATICO_8A93E8, 0x008A93E8);

CSF_FUNC(int, __cdecl, __stricmp, 0x007B0C41, (const char* left, const char* right));

static int __cdecl WeaponDefs_ParseMultiplayerAnimationType(char* String1)
{
    int entryIndex = 0;
    while (true)
    {
        const char* entryName = (&g_psz_ESTATICO_8A93E8)[entryIndex];
        if (entryName && __stricmp(String1, entryName) == 0)
            return entryIndex;

        if ((unsigned int)++entryIndex >= 0x18)
            return -1;
    }
}

HOOK(0x7618F0, WeaponDefs_ParseMultiplayerAnimationType, "auto");
