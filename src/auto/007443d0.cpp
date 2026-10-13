#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"

// Sets one of the extended multiplayer game-mode options. The option id selects
// a 32-bit slot in the object (ids 3..12 map to offsets 0x18, 0x14, 0x24, 0x20,
// 0x04, 0x0C, 0x08, 0x10, 0x28, 0x1C). Returns a3 for the ids that echo the
// value, otherwise optionId - 3, matching the original's eax handling.
static int __fastcall MpGameModes_SetExtendedOption(void* self, void* /*edx*/, int optionId, int value)
{
    int result = optionId - 3;

    switch (optionId)
    {
    case 3:
        CSF_AT(int, self, 0x18) = value;
        result = value;
        break;
    case 4:
        CSF_AT(int, self, 0x14) = value;
        break;
    case 5:
        CSF_AT(int, self, 0x24) = value;
        result = value;
        break;
    case 6:
        CSF_AT(int, self, 0x20) = value;
        break;
    case 7:
        CSF_AT(int, self, 0x04) = value;
        result = value;
        break;
    case 8:
        CSF_AT(int, self, 0x0C) = value;
        break;
    case 9:
        CSF_AT(int, self, 0x08) = value;
        result = value;
        break;
    case 10:
        CSF_AT(int, self, 0x10) = value;
        break;
    case 11:
        CSF_AT(int, self, 0x28) = value;
        result = value;
        break;
    case 12:
        CSF_AT(int, self, 0x1C) = value;
        break;
    default:
        return result;
    }
    return result;
}

HOOK(0x7443D0, MpGameModes_SetExtendedOption, "auto");
