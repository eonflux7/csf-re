#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Returns bit 2 of the byte at offset +2 of the RwFrame-like object.
static int __cdecl GfxFrames_GetFrameFlagBit2(int frame) {
    uint8_t flagsByte = CSF_AT(uint8_t, reinterpret_cast<void*>(frame), 0x2);
    return (flagsByte >> 2) & 1;
}

HOOK(0x006C0180, GfxFrames_GetFrameFlagBit2, "auto");
