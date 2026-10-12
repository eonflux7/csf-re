#include "globals.h"
#include "hook.h"

CSF_GLOBAL(const char*, g_psz_NINGUNO_app_config, 0x008A8E08);

CSF_FUNC(int, __cdecl, __stricmp, 0x007B0C41, (const char* left, const char* right));

// Scans the 4 character-type name entries in app_config for a case-insensitive match.
// Null entries are skipped. Returns the matching index, or 0 if none match.
static int __cdecl WeaponDefs_ParseCharacterType(char* String1)
{
    int typeIndex = 0;
    while (!(&g_psz_NINGUNO_app_config)[typeIndex] ||
           __stricmp(String1, (&g_psz_NINGUNO_app_config)[typeIndex]) != 0)
    {
        if ((unsigned int)++typeIndex >= 4)
            return 0;
    }
    return typeIndex;
}

HOOK(0x00761760, WeaponDefs_ParseCharacterType, "auto");
