#include <cstdint>
#include "hook.h"

// 0x0040D350 - leaf function that unconditionally returns the constant 18.
// Prototype: int sub_40D350()  (__cdecl, pops 0 bytes)
static int __cdecl Return18(void)
{
    return 18;
}

HOOK(0x40D350, Return18, "auto");
