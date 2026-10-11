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

// Returns bit 3 of the flags word at +0x58 when the pointer at +0x1C is non-null; otherwise 0.
static int __fastcall C_00861e98_IsBit3SetWhenLinked(C_00861e98* self, void* /*edx*/)
{
    if (CSF_AT(uint32_t, self, 0x1c) != 0)
    {
        return (CSF_AT(uint32_t, self, 0x58) >> 3) & 1;
    }
    return 0;
}

HOOK(0x006E8960, C_00861e98_IsBit3SetWhenLinked, "auto");
