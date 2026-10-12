#include "hook.h"

// Maps a dynamic object kind (0, 3, 4, 5, 6) to a small slot index.
// Any other kind returns -1.
static int __cdecl DynamicObj_MapKindToSlotIndex(int kind)
{
    int result = -1;
    switch (kind)
    {
    case 0:
        result = 5;
        break;
    case 3:
        result = 0;
        break;
    case 4:
        result = 2;
        break;
    case 5:
        result = 3;
        break;
    case 6:
        result = 4;
        break;
    default:
        return result;
    }
    return result;
}

HOOK(0x00442A20, DynamicObj_MapKindToSlotIndex, "auto");
