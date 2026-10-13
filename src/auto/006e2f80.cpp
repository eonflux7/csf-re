#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(char, __cdecl, NormalizeResourceName, 0x006949D0, (char* source, char* destination, unsigned int capacity));
CSF_FUNC(int, __cdecl, __stricmp, 0x007B0C41, (const char* left, const char* right));

static bool __cdecl GfxIm3d_IsSpecialResourceName(char* source)
{
    if (source && source[1] == 109 && source[2] == 112 && source[3] == 95)
        return true;

    alignas(8) char normalizedName[256];
    alignas(8) char suffix[256];

    NormalizeResourceName(source, normalizedName, 256);

    int length = 0;
    while (normalizedName[length] != 0)
        ++length;

    if (length <= 3)
        return false;

    suffix[0] = normalizedName[length - 3];
    suffix[1] = normalizedName[length - 2];
    suffix[2] = normalizedName[length - 1];
    suffix[3] = 0;

    return __stricmp(
        suffix,
        reinterpret_cast<const char*>(CSF_ADDR_DATA(0x00861A40))) == 0;
}

HOOK(0x006E2F80, GfxIm3d_IsSpecialResourceName, "auto");
