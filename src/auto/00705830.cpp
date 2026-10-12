#include <cstdint>
#include "hook.h"

// Returns true when the particle batch's blend/render class bits (0x100, 0x300, 0x500 in
// the flags word) are set and the second argument is greater than 8 (signed compare).
static bool __fastcall GfxParticle_IsSpecialBlendBatch(int16_t flags, int threshold)
{
    bool result = false;
    if (threshold > 8)
    {
        int blendClass = flags & 0xF00;
        if (blendClass == 0x100 || blendClass == 0x300 || blendClass == 0x500)
            return true;
    }
    return result;
}

HOOK(0x00705830, GfxParticle_IsSpecialBlendBatch, "auto");
