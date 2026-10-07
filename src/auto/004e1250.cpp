#include <cstdint>
#include "hook.h"

// Returns -1 unconditionally; the single stack argument is ignored.
static int __stdcall C_0083e160_ReturnMinusOne(int a1)
{
    (void)a1;
    return -1;
}

HOOK(0x004E1250, C_0083e160_ReturnMinusOne, "auto");
