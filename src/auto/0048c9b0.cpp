#include <cstdint>
#include "globals.h"
#include "hook.h"

// Clears the owner-subobject state block: dword at +0x20, word at +0x24,
// bytes at +0x38/+0x39, and sets the float 2.0f bit pattern at +0x3C.
static int __fastcall ResetOwnerSubobjectFields(void* self, void* /*edx*/) {
    CSF_AT(uint32_t, self, 0x20) = 0;
    CSF_AT(uint16_t, self, 0x24) = 0;
    CSF_AT(uint8_t, self, 0x38) = 0;
    CSF_AT(uint8_t, self, 0x39) = 0;
    CSF_AT(uint32_t, self, 0x3C) = 0x40000000u;
    return 0;
}

HOOK(0x48C9B0, ResetOwnerSubobjectFields, "auto");
