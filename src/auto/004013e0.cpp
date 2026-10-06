#include <cstdint>
#include "hook.h"

// Returns -1 unconditionally; likely a stub / default virtual implementation.
static int __cdecl ReturnMinusOne()
{
    return -1;
}

HOOK(0x4013E0, ReturnMinusOne, "auto");
