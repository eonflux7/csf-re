#include <cstdint>
#include "hook.h"

// Copies two 3-dword points: a2 -> this[0..2], a3 -> this[3..5].
// Returns a3[1], the value last stored (the original leaves it in eax).
static int __fastcall GfxBBox_CopyEndpointPair(uint32_t* self, void* /*edx*/, const uint32_t* a2, const uint32_t* a3)
{
    self[0] = a2[0];
    self[1] = a2[1];
    self[2] = a2[2];

    uint32_t* second = self + 3;
    second[0] = a3[0];
    int result = static_cast<int>(a3[1]);
    second[1] = static_cast<uint32_t>(result);
    second[2] = a3[2];
    return result;
}

HOOK(0x006AB460, GfxBBox_CopyEndpointPair, "auto");
