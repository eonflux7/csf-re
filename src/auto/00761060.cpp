#include <cstdint>
#include "hook.h"

// Returns 1 when the sub-object type id is one of the flagged kinds (6, 7, 8, 18, 19, 21), otherwise 0.
static int __cdecl IsSubObjectFlagSet(int a1)
{
    return a1 == 6 || a1 == 7 || a1 == 8 || a1 == 18 || a1 == 19 || a1 == 21;
}

HOOK(0x761060, IsSubObjectFlagSet, "auto");
