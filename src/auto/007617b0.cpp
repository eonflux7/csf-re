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

// Table of 0x38 const char* entries (animation type names, first entry "REPOSO").
CSF_GLOBAL(const char*, g_psz_REPOSO, 0x008A8E18);

CSF_FUNC(int, __cdecl, __stricmp, 0x007B0C41, (const char* left, const char* right));

static int __cdecl WeaponDefs_ParseAnimationType(char* String1)
{
    int entryIndex = 0;
    while (true)
    {
        const char* entryName = (&g_psz_REPOSO)[entryIndex];
        if (entryName && __stricmp(String1, entryName) == 0)
            return entryIndex;

        if ((unsigned int)++entryIndex >= 0x38)
            return -1;
    }
}

HOOK(0x7617B0, WeaponDefs_ParseAnimationType, "auto");
