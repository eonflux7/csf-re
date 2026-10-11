#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, RpWorldSectorRender, 0x005903F0, (int a1));

static int __cdecl GfxEnvironment_RenderWorldSectorAndReturnArg(int a1)
{
    RpWorldSectorRender(a1);
    return a1;
}

HOOK(0x006BC2B0, GfxEnvironment_RenderWorldSectorAndReturnArg, "auto");
