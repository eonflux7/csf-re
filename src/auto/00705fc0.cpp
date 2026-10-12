#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(char, __stdcall, sub_7A7B60, 0x007A7B60, (int a1));
CSF_FUNC(int, __fastcall, GetField04Bit1, 0x006B8160, (void* self, void*));

// Returns TRUE when the object passes the first check (sub_7A7B60) and its
// field-04 bit 1 is clear (GetField04Bit1 returns false).
static int __stdcall GfxParticle_IsRenderableUnflagged(C_0085ec18* a1)
{
    if (!sub_7A7B60(reinterpret_cast<int>(a1)))
        return 0;

    if ((char)GetField04Bit1(a1, nullptr) != 0)
        return 0;

    return 1;
}

HOOK(0x705FC0, GfxParticle_IsRenderableUnflagged, "auto");
