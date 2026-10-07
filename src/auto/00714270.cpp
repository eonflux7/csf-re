#include <cstdint>
#include "globals.h"
#include "hook.h"

// returns *(this + 0x1B8) + 2 * *(this + 0x170)
static int __fastcall C_00714270_ComputeDoubledFieldOffset(void* self, void* /*edx*/)
{
    int base = CSF_AT(int, self, 0x170);
    int extra = CSF_AT(int, self, 0x1B8);
    return extra + 2 * base;
}

HOOK(0x00714270, C_00714270_ComputeDoubledFieldOffset, "auto");
