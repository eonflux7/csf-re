#include <cstdint>
#include "globals.h"
#include "hook.h"

// Resets a 0x17-byte state block to its default values and returns the same pointer.
static uint32_t* __fastcall InitStateBlockDefaults(uint32_t* self, void* /*edx*/) {
    CSF_AT(uint32_t, self, 0x00) = 0;
    CSF_AT(uint32_t, self, 0x04) = 0;
    CSF_AT(uint32_t, self, 0x08) = 0;
    CSF_AT(uint32_t, self, 0x0C) = 0;
    CSF_AT(uint32_t, self, 0x10) = 0xFFFFFFFFu;
    CSF_AT(uint8_t, self, 0x14) = 0;
    CSF_AT(uint8_t, self, 0x15) = 1;
    CSF_AT(uint8_t, self, 0x16) = 0;
    return self;
}

HOOK(0x0045BC30, InitStateBlockDefaults, "auto");
