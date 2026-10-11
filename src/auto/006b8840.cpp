#include <cstdint>
#include "globals.h"
#include "hook.h"

// Reads the int stored at (base + a1), where base is the dword at 0x8BB478.
// The original reads the global fresh each call, and adds the signed argument
// to the base as a plain integer offset.
CSF_GLOBAL(uint32_t, g_DistanceOverrideBase, 0x8BB478);

static int __cdecl GfxSkin_GetDistanceOverrideOwner(int a1) {
    uint32_t address = g_DistanceOverrideBase + static_cast<uint32_t>(a1);
    return *reinterpret_cast<int*>(address);
}

HOOK(0x006B8840, GfxSkin_GetDistanceOverrideOwner, "auto");
