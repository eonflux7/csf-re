#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "legacy/string.h"

CSF_FUNC(void*, __cdecl, SceneLoader_PrepareObject, 0x006BB8A0, (uint32_t* a1));

static uint32_t* __cdecl GfxObjectProps_PrepareObjectAndReturnSelf(uint32_t* a1)
{
    SceneLoader_PrepareObject(a1);
    return a1;
}

HOOK(0x006BBBF0, GfxObjectProps_PrepareObjectAndReturnSelf, "auto");
