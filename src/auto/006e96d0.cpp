#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, HAnim_LookupChannelEntry, 0x006E9030, (void* self, void* edx, int a2));
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

// Returns 1 when the object has a channel table and the looked-up channel entry
// has a strictly positive float weight at +8; otherwise returns 0.
static int __fastcall C_00861e98_HasPositiveTrackWeight(C_00861e98* self, void* /*edx*/, int a2)
{
    if (CSF_AT(int, self, 0x1C) == 0)
        return 0;

    int channelEntry = HAnim_LookupChannelEntry(self, nullptr, a2);
    float weight = CSF_AT(float, reinterpret_cast<void*>(static_cast<uintptr_t>(channelEntry)), 8);
    if (weight > g_fZero)
        return 1;
    return 0;
}

HOOK(0x006E96D0, C_00861e98_HasPositiveTrackWeight, "auto");
