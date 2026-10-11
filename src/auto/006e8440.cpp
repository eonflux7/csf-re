#include <cstdint>
#include "hook.h"

// Returns non-zero when the HAnim track/channel type id is one of the
// values this module treats as a special-case type (40, 50, 60, 70, 71, 72, 100).
static int __cdecl IsHAnimSpecialTrackType(int trackType)
{
    switch (trackType) {
    case 40:
    case 50:
    case 60:
    case 70:
    case 71:
    case 72:
    case 100:
        return 1;
    default:
        return 0;
    }
}

HOOK(0x006E8440, IsHAnimSpecialTrackType, "auto");
