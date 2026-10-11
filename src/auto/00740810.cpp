#include <cstdint>
#include "globals.h"
#include "hook.h"

// Constructor-style initializer: stores the vtable and zeroes the member block.
// Returns `this` (mov eax, ecx at entry).
static uint8_t* __fastcall C_00867890_InitDefaults(void* self, void* /*edx*/)
{
    CSF_AT(void*, self, 0x00) = reinterpret_cast<void*>(CSF_VTBL(0x867890));
    CSF_AT(uint8_t, self, 0x04) = 0;
    CSF_AT(uint32_t, self, 0x08) = 0;
    CSF_AT(int32_t, self, 0x0C) = -1;
    CSF_AT(uint32_t, self, 0x10) = 0;
    CSF_AT(uint32_t, self, 0x14) = 0;
    CSF_AT(uint8_t, self, 0x18) = 0;
    return reinterpret_cast<uint8_t*>(self);
}

HOOK(0x740810, C_00867890_InitDefaults, "auto");
