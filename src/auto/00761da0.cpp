#include "globals.h"
#include "hook.h"

CSF_GLOBAL(const char*, g_psz_NINGUNO_app_config_2, 0x008A9688);

CSF_FUNC(int, __cdecl, __stricmp, 0x007B0C41, (const char* left, const char* right));

// Scans the 2 app-config name entries for a case-insensitive match.
// Null entries are skipped. Returns the matching slot index, or 0 if none match.
static int __cdecl CharacterDefs_FindAppConfigNameSlotIndex(char* name)
{
    int slotIndex = 0;
    while (!(&g_psz_NINGUNO_app_config_2)[slotIndex] ||
           __stricmp(name, (&g_psz_NINGUNO_app_config_2)[slotIndex]) != 0)
    {
        if ((unsigned int)++slotIndex >= 2)
            return 0;
    }
    return slotIndex;
}

HOOK(0x00761DA0, CharacterDefs_FindAppConfigNameSlotIndex, "auto");
