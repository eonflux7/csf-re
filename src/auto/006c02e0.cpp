#include <cstdint>
#include "hook.h"

// 0x006C02E0 (15 bytes)
// Stores the first argument through the caller-supplied output pointer and
// returns 0.  __cdecl: the caller cleans up the two stack arguments.
static int __cdecl StoreIntToOutPointerAndReturnZero(int value, int* out)
{
    *out = value;
    return 0;
}

HOOK(0x6C02E0, StoreIntToOutPointerAndReturnZero, "auto");
