#include "hook.h"

// Maps a small type code (0..12) to one of four light groups (0..3).
// Any value outside 0..12, including negatives, returns -1.
static int __cdecl SceneLights_GetGroupForCode(int code) {
    int result = -1;
    switch (code) {
    case 0:
    case 1:
        result = 0;
        break;
    case 2:
    case 3:
        result = 1;
        break;
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
        result = 2;
        break;
    case 12:
        result = 3;
        break;
    default:
        return result;
    }
    return result;
}

HOOK(0x004F0C50, SceneLights_GetGroupForCode, "auto");
