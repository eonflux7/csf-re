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

// Table of 4 const char* entries (distraccion type names).
CSF_GLOBAL(const char*, g_psz_DISTRACCION_8A962C, 0x008A962C);

CSF_FUNC(int, __cdecl, __stricmp, 0x007B0C41, (const char* left, const char* right));

static int __cdecl CharacterDefs_FindDistraccionIndex(char* String1)
{
    int entryIndex = 0;
    while (true)
    {
        const char* entryName = (&g_psz_DISTRACCION_8A962C)[entryIndex];
        if (entryName && __stricmp(String1, entryName) == 0)
            return entryIndex;

        if ((unsigned int)++entryIndex >= 4)
            return -1;
    }
}

HOOK(0x761C60, CharacterDefs_FindDistraccionIndex, "auto");
