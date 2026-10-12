#include <cstdint>
#include "hook.h"

// Returns `frame` unchanged unless its first byte is 1. In that case it stores
// `frame` into *outSlot and returns nullptr.
static uint8_t* __cdecl GfxFrames_TakeFlaggedFrame(uint8_t* frame, uint32_t* outSlot) {
    uint8_t* result = frame;
    if (*frame == 1) {
        *outSlot = reinterpret_cast<uint32_t>(frame);
        return nullptr;
    }
    return result;
}

HOOK(0x006C0270, GfxFrames_TakeFlaggedFrame, "auto");
