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

// Sets (enable != 0) or clears (enable == 0) the bits in bitMask in the camera flag word at +0x58.
// Returns bitMask when setting, or the updated flag word when clearing.
static int __fastcall CCamera_SetOrClearFlagBits(CCamera* self, void* /*edx*/, int bitMask, char enable)
{
    if (enable)
    {
        self->field_0058 |= static_cast<uint32_t>(bitMask);
        return bitMask;
    }

    int result = ~bitMask & static_cast<int>(self->field_0058);
    self->field_0058 = static_cast<uint32_t>(result);
    return result;
}

HOOK(0x6D2C90, CCamera_SetOrClearFlagBits, "auto");
