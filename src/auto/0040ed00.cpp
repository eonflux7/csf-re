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

// Sets (enable != 0) or clears (enable == 0) the bits in bitMask in the flag word at offset 0 of the object.
// Returns bitMask when setting, or the updated flag word when clearing.
static int __fastcall GfxObjectProps_SetOrClearFlagBits(void* self, void* /*edx*/, int bitMask, char enable)
{
    if (enable)
    {
        CSF_AT(int, self, 0) |= bitMask;
        return bitMask;
    }

    int result = ~bitMask & CSF_AT(int, self, 0);
    CSF_AT(int, self, 0) = result;
    return result;
}

HOOK_KEEP_ECX(0x40ED00, GfxObjectProps_SetOrClearFlagBits, 2, "auto");
