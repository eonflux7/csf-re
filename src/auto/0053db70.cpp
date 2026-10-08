#include <cstdint>
#include "globals.h"
#include "hook.h"

// Returns a pointer to the global settings block (the original's sub_4E8C50 result).
CSF_FUNC(uint8_t*, __cdecl, GetValue8B0554, 0x004E8C50, (void));

// Float scale constant (5.0) applied to the normalized position.
CSF_GLOBAL(float, g_LevelScaleFactor, 0x0082E048);

// Maps the value stored at self+0x8C, clamped to the [min,max] range read from the
// settings block, onto a level in 1..5. Pure computation, no side effects beyond the
// GetValue8B0554 call.
static int __fastcall MenuMissionInfo_ComputeScaledLevel(void* self, void* /*edx*/)
{
    uint8_t* settings = GetValue8B0554();
    int32_t rangeMax = CSF_AT(int32_t, settings, 0x1F38);
    int32_t rangeMin = CSF_AT(int32_t, settings, 0x1F3C);
    int32_t value = CSF_AT(int32_t, self, 0x8C);

    // Clamp value into [min(rangeMax, rangeMin), max(rangeMax, rangeMin)].
    // Note: the first bound comes from the 0x1F38 field, the second from 0x1F3C.
    if (rangeMin > rangeMax)
    {
        // Original: v4 (0x1F3C) > v3 (0x1F38) branch; here rangeMin is "v4".
        // Kept in the original's layout: v3 = rangeMax, v4 = rangeMin.
    }

    int32_t v3 = rangeMax; // value of [eax+0x1f38]
    int32_t v4 = rangeMin; // value of [eax+0x1f3c]
    int32_t v5 = value;

    if (v4 > v3)
    {
        if (v5 >= v3)
        {
            if (v5 > v4)
                v5 = v4;
        }
        else
        {
            v5 = v3;
        }
    }
    else
    {
        if (v5 < v4)
            v5 = v4;
        else if (v5 > v3)
            v5 = v3;
    }

    // Normalized position: (v5 - v4) / (v3 - v4) * 5.0, truncated toward zero.
    int32_t numerator = v5 - v4;
    int32_t denominator = v3 - v4;
    double ratio = static_cast<double>(numerator) / static_cast<double>(denominator);
    int64_t scaled = static_cast<int64_t>(ratio * g_LevelScaleFactor);

    uint32_t level = static_cast<uint32_t>(static_cast<int32_t>(scaled));
    if (level != 0)
    {
        if (level > 5)
            level = 5;
    }
    else
    {
        level = 1;
    }
    return static_cast<int>(level);
}

HOOK(0x0053DB70, MenuMissionInfo_ComputeScaledLevel, "auto");
