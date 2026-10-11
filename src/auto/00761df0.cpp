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

// Table of 3 const char* entries (app config names).
CSF_GLOBAL(const char*, g_psz_ESTATICO_app_config, 0x008A9690);

CSF_FUNC(int, __cdecl, __stricmp, 0x007B0C41, (const char* left, const char* right));

static int __cdecl CharacterDefs_FindAppConfigIndex(char* String1)
{
    int entryIndex = 0;
    while (true)
    {
        const char* entryName = (&g_psz_ESTATICO_app_config)[entryIndex];
        if (entryName && __stricmp(String1, entryName) == 0)
            return entryIndex;

        if ((unsigned int)++entryIndex >= 3)
            return -1;
    }
}

HOOK(0x761DF0, CharacterDefs_FindAppConfigIndex, "auto");
