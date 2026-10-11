#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Maps an entry kind id to a small tier value: 2 for the first group of ids,
// 1 for the second group, and 0 for everything else (including out-of-range ids).
static int __stdcall ClassifyEntryKindTier(int entryKind)
{
    int result = 0;
    switch (entryKind)
    {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 10:
    case 14:
        result = 2;
        break;
    case 11:
    case 12:
    case 19:
    case 20:
    case 21:
    case 22:
        result = 1;
        break;
    default:
        return result;
    }
    return result;
}

HOOK(0x004A6700, ClassifyEntryKindTier, "auto");
