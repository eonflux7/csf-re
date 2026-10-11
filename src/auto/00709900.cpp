#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Binds to the animation-frame setter called by the original (cdecl, caller pops).
CSF_FUNC(int, __cdecl, RpHAnimFrameSetID, 0x00593D30, (int a1, int a2, int a3));

// Unnamed helper the original calls first with (target, source) as cdecl arguments.
CSF_FUNC(int, __cdecl, ApplyWaterSourceState, 0x006B8860, (int a1, int a2));

// Copies the water-state word from source+0xF0 into target+0x70, applies the
// source state to the target, then sets the target's animation frame id to 0x10000.
// Returns the target pointer unchanged.
static int __cdecl WaterCopyStateAndSetFrameId(int target, int source)
{
    CSF_AT(int, target, 0x70) = CSF_AT(int, source, 0xF0);
    ApplyWaterSourceState(target, source);
    // The original pushes only two arguments here; the third slot is passed as 0.
    RpHAnimFrameSetID(CSF_AT(int, target, 0x18), 0x10000, 0);
    return target;
}

HOOK(0x709900, WaterCopyStateAndSetFrameId, "auto");
