#include <cstdint>
#include "globals.h"
#include "hook.h"

// Compares the first two dwords of two 8-byte point keys.
// Original: __thiscall, ecx = this, one stack arg (a2), ret 4.
static int __fastcall WaypointGraph_ArePointKeysEqual(void* self, void* /*edx*/, void* other)
{
    const uint32_t firstThis = CSF_AT(uint32_t, self, 0);
    const uint32_t firstOther = CSF_AT(uint32_t, other, 0);
    if (firstThis != firstOther) {
        return 0;
    }
    const uint32_t secondThis = CSF_AT(uint32_t, self, 4);
    const uint32_t secondOther = CSF_AT(uint32_t, other, 4);
    return secondThis == secondOther ? 1 : 0;
}

HOOK(0x004A9BF0, WaypointGraph_ArePointKeysEqual, "auto");
