#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "core/memory.h"

// Resets a 0x4C-byte record to its default state. Returns the same object.
// Field types are not declared for this block, so every access uses CSF_AT.
static void* __fastcall InitDefaultFieldBlock(void* self, void* /*edx*/) {
    CSF_AT(uint8_t, self, 0x00) = 0;
    CSF_AT(int, self, 0x04) = 0;
    CSF_AT(int, self, 0x08) = 0;
    CSF_AT(int, self, 0x0C) = 0;
    CSF_AT(int, self, 0x10) = 0;
    CSF_AT(int, self, 0x14) = 10;
    CSF_AT(float, self, 0x18) = -1.0f;
    CSF_AT(int, self, 0x1C) = 0;
    CSF_AT(int, self, 0x20) = 0;
    CSF_AT(int, self, 0x24) = 0;
    CSF_AT(int, self, 0x28) = 0;
    CSF_AT(int, self, 0x2C) = 0;
    CSF_AT(int, self, 0x30) = 0;
    CSF_AT(int, self, 0x34) = 0;
    CSF_AT(int, self, 0x38) = 0;
    CSF_AT(int, self, 0x3C) = 0;
    CSF_AT(int, self, 0x40) = 0;
    CSF_AT(int, self, 0x44) = 0;
    CSF_AT(int, self, 0x48) = 10;
    return self;
}

HOOK(0x004DEF80, InitDefaultFieldBlock, "auto");
