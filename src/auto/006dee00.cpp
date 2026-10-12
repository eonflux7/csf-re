#include "hook.h"
#include "globals.h"

CSF_FUNC(int, __cdecl, GfxTextureLoader_ProcessTexture, 0x006DEC80,
         (int a1, int a2, int a3, int a4));

static int __cdecl GfxTextureLoader_ForwardTextureRequest(int a1, int a2, int a3)
{
    // The original forwards an uninitialized stack slot as the fourth argument.
    return GfxTextureLoader_ProcessTexture(a1, a2, a3, 0);
}

HOOK(0x006DEE00, GfxTextureLoader_ForwardTextureRequest, "auto");
