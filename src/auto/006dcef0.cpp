#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, __stricmp, 0x007B0C41, (const char* left, const char* right));
CSF_GLOBAL(const char*, g_psz_BOX, 0x008A8048);

static int __cdecl GfxTextureLoader_FindFilterType(char* name)
{
    for (int index = 0; index < 3; ++index)
    {
        if (name && (&g_psz_BOX)[index] &&
            __stricmp(name, (&g_psz_BOX)[index]) == 0)
        {
            return index;
        }
    }
    return -1;
}

HOOK(0x006DCEF0, GfxTextureLoader_FindFilterType, "auto");
