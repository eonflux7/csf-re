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

// Table of 3 const char* entries (no-recharge message strings).
CSF_GLOBAL(const char*, g_psz_NO_RECARGUES_POR_TU_CUENTA, 0x008A963C);

CSF_FUNC(int, __cdecl, __stricmp, 0x007B0C41, (const char* left, const char* right));

static int __cdecl CharacterDefs_FindNoRechargeMessageIndex(char* String1)
{
    int entryIndex = 0;
    while (true)
    {
        const char* entryName = (&g_psz_NO_RECARGUES_POR_TU_CUENTA)[entryIndex];
        if (entryName && __stricmp(String1, entryName) == 0)
            return entryIndex;

        if ((unsigned int)++entryIndex >= 3)
            return -1;
    }
}

HOOK(0x761CB0, CharacterDefs_FindNoRechargeMessageIndex, "auto");
