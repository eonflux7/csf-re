#include <cstdint>
#include "globals.h"
#include "hook.h"

// Zeroes nine dword fields of an object (no class declared for this block).
// Writes follow the original order: +0x0C, +0x1C, +0x2C, +0x3C, +0x44, +0x48, +0x4C, +0x54, +0x50.
static int __fastcall ResetObjectFieldBlockToZero(void* self, void* /*edx*/)
{
    CSF_AT(int, self, 0x0C) = 0;
    CSF_AT(int, self, 0x1C) = 0;
    CSF_AT(int, self, 0x2C) = 0;
    CSF_AT(int, self, 0x3C) = 0;
    CSF_AT(int, self, 0x44) = 0;
    CSF_AT(int, self, 0x48) = 0;
    CSF_AT(int, self, 0x4C) = 0;
    CSF_AT(int, self, 0x54) = 0;
    CSF_AT(int, self, 0x50) = 0;
    return 0;
}

HOOK(0x6AEF20, ResetObjectFieldBlockToZero, "auto");
