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

// Sets (enable != 0) or clears (enable == 0) the bits in bitMask in the light object's flag word (+0x40).
// Returns bitMask when setting, or the updated flag word when clearing.
static int __fastcall C_00862e88_SetOrClearFlagBits(C_00862e88* self, void* /*edx*/, int bitMask, char enable)
{
    if (enable)
    {
        self->field_0040 |= bitMask;
        return bitMask;
    }

    int result = ~bitMask & self->field_0040;
    self->field_0040 = result;
    return result;
}

HOOK(0x6FC550, C_00862e88_SetOrClearFlagBits, "auto");
