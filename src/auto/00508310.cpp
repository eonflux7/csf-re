#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Constructor-style initializer for a UI sub-object: zeroes the state block,
// sets the scale-like float at +0x2C to 1.0f, and sets the three size/limit
// dwords at +0x40, +0x50, +0x60 to 10. Returns the object itself.
static void* __fastcall UiSubObject_Construct(void* self, void* /*edx*/)
{
    CSF_AT(uint32_t, self, 0x04) = 0;
    CSF_AT(uint32_t, self, 0x08) = 0;
    CSF_AT(uint32_t, self, 0x20) = 0;
    CSF_AT(uint32_t, self, 0x24) = 0;
    CSF_AT(uint32_t, self, 0x28) = 0;
    CSF_AT(uint32_t, self, 0x2C) = 1065353216;
    CSF_AT(uint32_t, self, 0x34) = 0;
    CSF_AT(uint32_t, self, 0x38) = 0;
    CSF_AT(uint32_t, self, 0x3C) = 0;
    CSF_AT(uint32_t, self, 0x40) = 10;
    CSF_AT(uint32_t, self, 0x44) = 0;
    CSF_AT(uint32_t, self, 0x48) = 0;
    CSF_AT(uint32_t, self, 0x4C) = 0;
    CSF_AT(uint32_t, self, 0x50) = 10;
    CSF_AT(uint32_t, self, 0x54) = 0;
    CSF_AT(uint32_t, self, 0x58) = 0;
    CSF_AT(uint32_t, self, 0x5C) = 0;
    CSF_AT(uint32_t, self, 0x60) = 10;
    CSF_AT(uint8_t, self, 0x00) = 0;
    return self;
}

HOOK(0x00508310, UiSubObject_Construct, "auto");
