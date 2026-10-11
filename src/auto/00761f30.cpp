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

// Table of 8 const char* entries (particle system names).
CSF_GLOBAL(const char*, g_psz_PARTICLE_SYSTEM, 0x008A96D0);

CSF_FUNC(int, __cdecl, __stricmp, 0x007B0C41, (const char* left, const char* right));

static int __cdecl CharacterDefs_FindParticleSystemIndex(char* String1)
{
    int entryIndex = 0;
    while (true)
    {
        const char* entryName = (&g_psz_PARTICLE_SYSTEM)[entryIndex];
        if (entryName && __stricmp(String1, entryName) == 0)
            return entryIndex;

        if ((unsigned int)++entryIndex >= 8)
            return -1;
    }
}

HOOK(0x761F30, CharacterDefs_FindParticleSystemIndex, "auto");
