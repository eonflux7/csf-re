#include <cstdint>
#include "globals.h"
#include "hook.h"

// Sets or clears bit 0x20 in the 32-bit flags word at +0x04 and returns the updated word.
static uint32_t __fastcall SceneLoader_SetAttachmentFlagBit20(void* self, void* /*edx*/, char enable) {
    uint32_t flags = CSF_AT(uint32_t, self, 0x4);
    if (enable) {
        flags = flags | 0x20u;
    } else {
        flags = flags & 0xFFFFFFDFu;
    }
    CSF_AT(uint32_t, self, 0x4) = flags;
    return flags;
}

HOOK(0x6B8C10, SceneLoader_SetAttachmentFlagBit20, "auto");
