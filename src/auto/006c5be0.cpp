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

// Sets (enable != 0) or clears (enable == 0) the bits in bitMask in the object's flag word at +0x08.
// Returns the bitMask when setting, or the updated flag word when clearing.
static int __fastcall C_008656d0_SetOrClearFlagBits(C_008656d0* self, void* /*edx*/, int bitMask, char enable)
{
    if (enable)
    {
        self->field_0008 |= static_cast<uint32_t>(bitMask);
        return bitMask;
    }

    int remainingFlags = static_cast<int>(self->field_0008) & ~bitMask;
    self->field_0008 = static_cast<uint32_t>(remainingFlags);
    return remainingFlags;
}

HOOK(0x6C5BE0, C_008656d0_SetOrClearFlagBits, "auto");
