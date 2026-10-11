#include <cstdint>
#include <cstring>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Enables or disables this audio effect. When enabling, it latches the current
// float level (as raw bits) into field_0030 the first time, and clears field_002c.
// When disabling, it sets field_0040. Returns the low byte of the final eax value.
static char __fastcall C_008639b0_SetPlayingState(C_008639b0* self, void* /*edx*/, char enable)
{
    uint32_t result = static_cast<uint8_t>(enable);

    if (enable)
    {
        result = 1;
        if (self->field_0028 != 1)
        {
            self->field_0028 = 1;
            uint32_t levelBits = 0;
            std::memcpy(&levelBits, &self->field_0004, sizeof(levelBits));
            result = levelBits;
            self->field_0030 = levelBits;
        }
        self->field_002c = 0;
    }
    else
    {
        self->field_0040 = 1;
    }

    return static_cast<char>(result);
}

HOOK(0x0070E040, C_008639b0_SetPlayingState, "auto");
