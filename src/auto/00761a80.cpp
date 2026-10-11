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

// Table of 0x1A const char* entries (NINGUNO-terminated name table, "none" = first slot).
CSF_GLOBAL(const char*, g_psz_NINGUNO_8A9560, 0x008A9560);

CSF_FUNC(int, __cdecl, __stricmp, 0x007B0C41, (const char* left, const char* right));

static int __cdecl CharacterDefs_FindNameTableIndex(char* String1)
{
    int entryIndex = 0;
    while (true)
    {
        const char* entryName = (&g_psz_NINGUNO_8A9560)[entryIndex];
        if (entryName && __stricmp(String1, entryName) == 0)
            return entryIndex;

        if ((unsigned int)++entryIndex >= 0x1A)
            return -1;
    }
}

HOOK(0x761A80, CharacterDefs_FindNameTableIndex, "auto");
