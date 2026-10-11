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

// Table of 0x16 const char* entries (vehicle boarding action names).
CSF_GLOBAL(const char*, g_psz_SUBIR_VEHICULO, 0x008A9448);

CSF_FUNC(int, __cdecl, __stricmp, 0x007B0C41, (const char* left, const char* right));

static int __cdecl CharacterDefs_FindBoardVehicleIndex(char* String1)
{
    int entryIndex = 0;
    while (true)
    {
        const char* entryName = (&g_psz_SUBIR_VEHICULO)[entryIndex];
        if (entryName && __stricmp(String1, entryName) == 0)
            return entryIndex;

        if ((unsigned int)++entryIndex >= 0x16)
            return -1;
    }
}

HOOK(0x761940, CharacterDefs_FindBoardVehicleIndex, "auto");
