#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Maps a type id to a small tier value: 2 for the first group of ids,
// 1 for the second group, and 0 for everything else.
static int __stdcall ClassifyTypeIdTier(int typeId)
{
    int result = 0;
    switch (typeId)
    {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 23:
        result = 2;
        break;
    case 7:
    case 8:
    case 9:
    case 20:
    case 21:
    case 22:
    case 54:
    case 55:
        result = 1;
        break;
    default:
        return result;
    }
    return result;
}

HOOK(0x004A2E00, ClassifyTypeIdTier, "auto");
