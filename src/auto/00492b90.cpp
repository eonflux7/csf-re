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

// Returns 1 when bit 0x4 of the object's flags word at +0x1F0 is set, otherwise 0.
static int __fastcall IsEligibleNearbyCandidate(void* self, void* /*edx*/)
{
    uint32_t flags = CSF_AT(uint32_t, self, 0x1F0);
    return (flags & 4) == 4 ? 1 : 0;
}

HOOK(0x00492B90, IsEligibleNearbyCandidate, "auto");
