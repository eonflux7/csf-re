#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, RwTextureDestroy, 0x006317E0, (int texture));

static int __cdecl GfxTextureDict_DestroyAndReturnTexture(int texture)
{
    RwTextureDestroy(texture);
    return texture;
}

HOOK(0x006C0FD0, GfxTextureDict_DestroyAndReturnTexture, "auto");
