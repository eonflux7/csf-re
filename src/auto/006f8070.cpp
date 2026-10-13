#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "legacy/string.h"

CSF_FUNC(void, __cdecl, SceneLoader_NoOp, 0x00506AC0, ());

static int __cdecl GfxMaterials_CallNoOpAndReturnArg(int a1)
{
    SceneLoader_NoOp();
    return a1;
}

HOOK(0x006F8070, GfxMaterials_CallNoOpAndReturnArg, "auto");
