#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(float, g_WeaponPlacementTraceDepth, 0x0082E11C);

// Scans a table of 28-byte entries owned by `self` (self[0] = entry base, self[2] = entry count).
// Returns 0 when an active entry with a different owner id lies closer than the trace depth to
// the position; otherwise returns 1.
static char __fastcall AiFncStatic_CheckPositionPair(uint32_t* self, void* /*edx*/,
                                                    const uint32_t* requesterRef,
                                                    const float* position)
{
    const uint32_t entryCount = self[2];
    if (entryCount == 0)
        return 1;

    uint8_t* entry = reinterpret_cast<uint8_t*>(self[0]);
    // Cursor mirrors the original's ecx: it starts at entry + 0x18 and advances by 0x1C.
    uint8_t* cursor = entry + 0x18;
    uint32_t index = 0;
    do {
        const uint32_t activeFlag = CSF_AT(uint32_t, entry, 0x0);
        const uint32_t ownerId = CSF_AT(uint32_t, cursor, -0x10);
        if (activeFlag != 0 && ownerId != CSF_AT(uint32_t, requesterRef, 0x4)) {
            const float dx = CSF_AT(float, cursor, -0x8) - position[0];
            const float dy = CSF_AT(float, cursor, -0x4) - position[1];
            const float dz = CSF_AT(float, cursor, 0x0) - position[2];
            const float distanceSquared = dx * dx + dy * dy + dz * dz;
            if (distanceSquared < g_WeaponPlacementTraceDepth)
                return 0;
        }
        entry += 0x1C;
        cursor += 0x1C;
        ++index;
    } while (index < entryCount);

    return 1;
}

HOOK(0x004DC660, AiFncStatic_CheckPositionPair, "auto");
