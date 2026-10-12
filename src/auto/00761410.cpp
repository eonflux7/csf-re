#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, __stricmp, 0x007B0C41, (const char* left, const char* right));

// Maps a HUD texture entry name to its index: PRECISION=0, PRISMATICOS=1, MASCARA=2, PUERTA=3.
// Returns -1 for a null name or any unknown name.
static int __cdecl Hud_FindTextureEntryIndex(char* string1)
{
    if (string1 == nullptr)
        return -1;
    if (__stricmp(string1, CSF_STR(0x0086C584, "PRECISION")) == 0)
        return 0;
    if (__stricmp(string1, CSF_STR(0x0086C578, "PRISMATICOS")) == 0)
        return 1;
    if (__stricmp(string1, CSF_STR(0x0086C570, "MASCARA")) == 0)
        return 2;
    if (__stricmp(string1, CSF_STR(0x00860570, "PUERTA")) == 0)
        return 3;
    return -1;
}

HOOK(0x00761410, Hud_FindTextureEntryIndex, "auto");
