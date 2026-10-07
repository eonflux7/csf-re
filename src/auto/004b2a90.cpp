#include <cstdint>
#include "hook.h"

// 0x004B2A90 - trivial constant-returning accessor (returns 31 / 0x1F).
// The original is a plain function: it pops no stack bytes (`ret` with no immediate).
static int __cdecl GetConstant31()
{
    return 31;
}

HOOK(0x4B2A90, GetConstant31, "auto");
