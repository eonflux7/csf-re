#include <cstdint>
#include "hook.h"

// Increments the counter pointed to by the second argument, then returns the
// first argument unchanged. Matches the original 13-byte __cdecl stub.
static int __cdecl IncrementCounterAndReturnValue(int value, uint32_t* counter)
{
    ++*counter;
    return value;
}

HOOK(0x006EE5A0, IncrementCounterAndReturnValue, "auto");
