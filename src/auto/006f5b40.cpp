#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Stores the argument at +0x0C, sets the byte flag at +0x3C, and returns the argument.
static int __fastcall GfxCamera_StoreValueAndSetDirtyFlag(void* self, void* /*edx*/, int value) {
    CSF_AT(uint32_t, self, 0x0C) = static_cast<uint32_t>(value);
    CSF_AT(uint8_t, self, 0x3C) = 1;
    return value;
}

HOOK(0x006F5B40, GfxCamera_StoreValueAndSetDirtyFlag, "auto");
