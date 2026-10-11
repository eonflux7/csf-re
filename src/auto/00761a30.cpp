#include "globals.h"
#include "hook.h"

CSF_GLOBAL(const char*, g_psz_NINGUNO_8A9544, 0x008A9544);

CSF_FUNC(int, __cdecl, __stricmp, 0x007B0C41, (const char* left, const char* right));

// Scans the 7 character-slot name entries for a case-insensitive match.
// Null entries are skipped. Returns the matching slot index, or 0 if none match.
static int __cdecl CharacterDefs_FindNameSlotIndex(char* name)
{
    int slotIndex = 0;
    while (!(&g_psz_NINGUNO_8A9544)[slotIndex] ||
           __stricmp(name, (&g_psz_NINGUNO_8A9544)[slotIndex]) != 0)
    {
        if ((unsigned int)++slotIndex >= 7)
            return 0;
    }
    return slotIndex;
}

HOOK(0x00761A30, CharacterDefs_FindNameSlotIndex, "auto");
