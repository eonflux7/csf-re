#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"

// Clears the 32-bit value at +0x190 and sets the byte flag at +0x92.
static void __fastcall GfxScene_ActivateRenderResources(void* self, void* /*edx*/) {
    CSF_AT(int, self, 0x190) = 0;
    CSF_AT(uint8_t, self, 0x92) = 1;
}

HOOK(0x006BC080, GfxScene_ActivateRenderResources, "auto");
