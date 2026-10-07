#include <cstdint>
#include "hook.h"

// Returns the 64-bit value -1 (edx:eax = 0xFFFFFFFF:0xFFFFFFFF).
// The single stack argument is unused (ret 4).
static int64_t __stdcall ReturnMinusOne(int /*a1*/)
{
    return -1;
}

HOOK(0x6A7190, ReturnMinusOne, "auto");
