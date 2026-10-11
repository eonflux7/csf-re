#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, sub_6C01C0, 0x006C01C0, (int a1, int a2));

static int __stdcall GfxSkin_ForwardWithSkinCallback(int a1)
{
    return sub_6C01C0(a1, static_cast<int>(CSF_ADDR_FN(0x006C5C90)));
}

HOOK(0x006C5CB0, GfxSkin_ForwardWithSkinCallback, "auto");
