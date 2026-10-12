#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, __stricmp, 0x007B0C41, (const char* left, const char* right));
CSF_GLOBAL(const char*, g_psz_NONE, 0x008A9E68);

static int __stdcall CollisionShapes_FindTypeIndexByName(char* String1)
{
    int index = 0;
    while (true)
    {
        const char* entry = (&g_psz_NONE)[index];
        if (String1 && entry && __stricmp(String1, entry) == 0)
            return index;
        if (++index >= 22)
            return -1;
    }
}

HOOK(0x006CFA00, CollisionShapes_FindTypeIndexByName, "auto");
