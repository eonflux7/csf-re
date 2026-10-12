#include "globals.h"
#include "hook.h"

CSF_GLOBAL(const char*, g_psz_NINGUNO_8A8DF8, 0x008A8DF8);

CSF_FUNC(int, __cdecl, __stricmp, 0x007B0C41, (const char* left, const char* right));

// Scans the 4 weight-class name entries for a case-insensitive match.
// Null entries are skipped. Returns the matching class index, or 0 if none match.
static int __cdecl WeaponDefs_ParseWeightClass(char* name)
{
    int classIndex = 0;
    while (!(&g_psz_NINGUNO_8A8DF8)[classIndex] ||
           __stricmp(name, (&g_psz_NINGUNO_8A8DF8)[classIndex]) != 0)
    {
        if ((unsigned int)++classIndex >= 4)
            return 0;
    }
    return classIndex;
}

HOOK(0x00761710, WeaponDefs_ParseWeightClass, "auto");
