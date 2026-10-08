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

// Returns the global context pointer (__cdecl, no arguments).
CSF_FUNC(int, __cdecl, GetGlobalValue_8B056C, 0x506BD0, ());

// Returns 0 when the id matches one of the two tracked slots (slot A: 0x2E8/0x2EC,
// slot B: 0x2D4/0x2D8, each slot only compared when its occupancy word is zero),
// otherwise 1.
static int __stdcall MenuControls_IsIdNotInTwoSlots(int id)
{
    uint8_t* context = reinterpret_cast<uint8_t*>(GetGlobalValue_8B056C());
    uint8_t* state = CSF_AT(uint8_t*, context, 0x1C);

    // Slot A: matches only when its occupancy word is zero and its id equals the argument.
    bool slotAOccupied = CSF_AT(uint32_t, state, 0x2E8) != 0;
    bool slotAMismatch = slotAOccupied || CSF_AT(uint32_t, state, 0x2EC) != static_cast<uint32_t>(id);
    if (!slotAMismatch) {
        return 0;
    }

    // Slot B: same test on its own fields.
    bool slotBOccupied = CSF_AT(uint32_t, state, 0x2D4) != 0;
    bool slotBMismatch = slotBOccupied || CSF_AT(uint32_t, state, 0x2D8) != static_cast<uint32_t>(id);
    return slotBMismatch ? 1 : 0;
}

HOOK(0x00542BF0, MenuControls_IsIdNotInTwoSlots, "auto");
