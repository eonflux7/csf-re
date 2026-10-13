#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"

// Maps a 1-based action id (1..16) to a small code value.
// Unlisted ids (including 0 and anything above 16) return 0.
static int __stdcall MapActionIdToCode(int a1)
{
    int result = 0;

    switch (a1)
    {
    case 1:
        result = 2;
        break;
    case 2:
        result = 0;
        break;
    case 3:
    case 4:
        result = 1;
        break;
    case 5:
        result = 4;
        break;
    case 6:
    case 7:
    case 13:
        result = 8;
        break;
    case 8:
        result = 6;
        break;
    case 9:
    case 10:
        result = 10;
        break;
    case 11:
        result = 3;
        break;
    case 12:
        result = 7;
        break;
    case 14:
        result = 5;
        break;
    case 15:
    case 16:
        result = 9;
        break;
    default:
        return result;
    }
    return result;
}

HOOK(0x401100, MapActionIdToCode, "auto");
