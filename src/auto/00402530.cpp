#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Resets the object's state fields to their default values and returns the object.
// __thiscall rewritten as __fastcall (ecx = this, edx unused); no stack arguments, so ret 0.
static void* __fastcall ResetObjectStateFields(void* self, void* /*edx*/)
{
    CSF_AT(int, self, 0x0C) = 0;
    CSF_AT(int, self, 0x10) = 0;
    CSF_AT(int, self, 0x14) = 0;
    CSF_AT(int, self, 0x24) = 0;
    CSF_AT(int, self, 0x28) = 0;
    CSF_AT(int, self, 0x2C) = 0;
    CSF_AT(int, self, 0x18) = 0;
    CSF_AT(int, self, 0x1C) = 0;
    CSF_AT(int, self, 0x20) = 0;
    CSF_AT(int, self, 0x24) = 0x3F800000;
    CSF_AT(int, self, 0x28) = 0;
    CSF_AT(int, self, 0x2C) = 0;
    CSF_AT(uint8_t, self, 0x40) = 1;
    CSF_AT(int, self, 0x44) = 0;
    CSF_AT(int, self, 0x48) = 0;
    CSF_AT(int, self, 0x4C) = 0;
    CSF_AT(int, self, 0x50) = 0;
    CSF_AT(int, self, 0x54) = 0;
    CSF_AT(int, self, 0x58) = 0;
    CSF_AT(int, self, 0x5C) = 0;
    CSF_AT(int, self, 0x60) = 0;
    CSF_AT(int, self, 0x64) = 0;
    CSF_AT(int, self, 0x84) = 0;
    CSF_AT(int, self, 0x88) = 0;
    CSF_AT(int, self, 0x8C) = 0;
    CSF_AT(int, self, 0x80) = 0;
    return self;
}

HOOK(0x402530, ResetObjectStateFields, "auto");
