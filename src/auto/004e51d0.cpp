#include "hook.h"

// Maps a small slot index (0..14) to one of four groups (0..3).
// Any value outside 0..14, including negatives, returns -1.
static int __cdecl GetGroupIndexForSlot(int slot) {
    int result = -1;
    switch (slot) {
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
        result = 2;
        break;
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
        result = 3;
        break;
    default:
        return result;
    }
    return result;
}

HOOK(0x004E51D0, GetGroupIndexForSlot, "auto");
