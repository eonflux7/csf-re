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

// Table of 3 const char* entries (range type names: "CORTO", "MEDIO", "LARGO" set).
CSF_GLOBAL(const char*, g_psz_CORTO, 0x008A8DE4);

CSF_FUNC(int, __cdecl, __stricmp, 0x007B0C41, (const char* left, const char* right));

static int __cdecl WeaponDefs_ParseRangeType(char* String1)
{
    int entryIndex = 0;
    while (true)
    {
        const char* entryName = (&g_psz_CORTO)[entryIndex];
        if (entryName && __stricmp(String1, entryName) == 0)
            break;

        if ((unsigned int)++entryIndex >= 3)
            return -1;
    }
    return entryIndex;
}

HOOK(0x761670, WeaponDefs_ParseRangeType, "auto");
