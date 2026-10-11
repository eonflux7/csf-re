#include "globals.h"
#include "hook.h"

CSF_GLOBAL(const char*, g_psz_NINGUNO_8A95F8, 0x008A95F8);

CSF_FUNC(int, __cdecl, __stricmp, 0x007B0C41, (const char* left, const char* right));

// Scans the 6 grade-name entries for a case-insensitive match.
// Null entries are skipped. Returns the matching slot index, or 0 if none match.
static int __cdecl CharacterDefs_FindGradeIndex(char* name)
{
    int slotIndex = 0;
    while (!(&g_psz_NINGUNO_8A95F8)[slotIndex] ||
           __stricmp(name, (&g_psz_NINGUNO_8A95F8)[slotIndex]) != 0)
    {
        if ((unsigned int)++slotIndex >= 6)
            return 0;
    }
    return slotIndex;
}

HOOK(0x00761B70, CharacterDefs_FindGradeIndex, "auto");
