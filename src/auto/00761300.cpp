#include <cstdint>
#include "hook.h"

// Returns the region id when it is one of the known values 1..3, otherwise 0.
static int __cdecl NormalizeRegionCode(int regionId)
{
    int result = 0;
    switch (regionId)
    {
    case 1:
        return 1;
    case 2:
        return 2;
    case 3:
        return 3;
    }
    return result;
}

HOOK(0x00761300, NormalizeRegionCode, "auto");
