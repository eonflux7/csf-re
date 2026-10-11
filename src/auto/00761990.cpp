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

// Table of const char* names for destruction types (only entry 0 is consulted).
CSF_GLOBAL(const char*, g_psz_DESTRUCCION, 0x008A94A0);

CSF_FUNC(int, __cdecl, __stricmp, 0x007B0C41, (const char* left, const char* right));

static int __cdecl CharacterDefs_ParseDestructionType(char* String1)
{
    const char* entryName = (&g_psz_DESTRUCCION)[0];
    if (entryName && __stricmp(String1, entryName) == 0)
        return 0;
    return -1;
}

HOOK(0x761990, CharacterDefs_ParseDestructionType, "auto");
