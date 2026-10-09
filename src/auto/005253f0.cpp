#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"

// Classifies a sound type id. Returns 1 for the ids in the "yes" set
// (including -1, 59, 60, 61), 0 for the explicit "no" set (56, 57, 58)
// and for every other value.
static char __stdcall IsSoundTypeInSecondFlagSet(int soundType)
{
    char result = 0;
    switch (soundType)
    {
    case -1:
    case 9:
    case 10:
    case 14:
    case 15:
    case 16:
    case 59:
    case 60:
    case 61:
        result = 1;
        break;
    case 56:
    case 57:
    case 58:
        result = 0;
        break;
    default:
        return result;
    }
    return result;
}

HOOK(0x005253F0, IsSoundTypeInSecondFlagSet, "auto");
