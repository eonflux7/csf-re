#include "globals.h"
#include "hook.h"

CSF_GLOBAL(const char*, g_psz_SMG, 0x008A8DC0);

CSF_FUNC(int, __cdecl, __stricmp, 0x007B0C41, (const char* left, const char* right));

// Scans the 3 weapon aim-type name entries for a case-insensitive match.
// Null entries are skipped. Returns the matching index, or 0 if none match.
static int __cdecl WeaponDefs_ParseAimType(char* String1)
{
    int aimTypeIndex = 0;
    while (!(&g_psz_SMG)[aimTypeIndex] ||
           __stricmp(String1, (&g_psz_SMG)[aimTypeIndex]) != 0)
    {
        if ((unsigned int)++aimTypeIndex >= 3)
            return 0;
    }
    return aimTypeIndex;
}

HOOK(0x00761580, WeaponDefs_ParseAimType, "auto");
