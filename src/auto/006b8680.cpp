#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, sub_6BB960, 0x006BB960, (void** a1));

static void** __cdecl GfxObjectProps_ProcessAndReturnSelf(void** a1)
{
    sub_6BB960(a1);
    return a1;
}

HOOK(0x006B8680, GfxObjectProps_ProcessAndReturnSelf, "auto");
