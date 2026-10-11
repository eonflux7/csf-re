#include <cstdint>
#include "globals.h"
#include "hook.h"

// Global base pointer (dword_8BB478) used as the origin for the water-source state slot.
CSF_GLOBAL(uint8_t*, g_WaterSourceStateBase, 0x8BB478);

// Stores `value` as a 32-bit integer at (base + offset) and returns `value`.
static int __cdecl GfxObjectProps_ApplyWaterSourceState(int offset, int value)
{
    *reinterpret_cast<int*>(g_WaterSourceStateBase + offset) = value;
    return value;
}

HOOK(0x006B8860, GfxObjectProps_ApplyWaterSourceState, "auto");
