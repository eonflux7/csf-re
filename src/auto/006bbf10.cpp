#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, GfxObjectProps_ProcessProps, 0x006BBC10, (int a1));

static int __cdecl GfxObjectProps_ProcessAndReturnArg(int a1)
{
    GfxObjectProps_ProcessProps(a1);
    return a1;
}

HOOK(0x006BBF10, GfxObjectProps_ProcessAndReturnArg, "auto");
