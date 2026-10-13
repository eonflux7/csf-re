#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Returns one 32-bit setting from the multiplayer game-mode/connection object.
// Selector 3..12 maps to a fixed dword slot; any other selector returns 0.
static int __fastcall MpGameModes_GetConnectionSetting(void* self, void* /*edx*/, int a2)
{
    int result = 0;
    switch (a2) {
    case 3:
        result = CSF_AT(int, self, 0x18);
        break;
    case 4:
        result = CSF_AT(int, self, 0x14);
        break;
    case 5:
        result = CSF_AT(int, self, 0x24);
        break;
    case 6:
        result = CSF_AT(int, self, 0x20);
        break;
    case 7:
        result = CSF_AT(int, self, 0x04);
        break;
    case 8:
        result = CSF_AT(int, self, 0x0C);
        break;
    case 9:
        result = CSF_AT(int, self, 0x08);
        break;
    case 10:
        result = CSF_AT(int, self, 0x10);
        break;
    case 11:
        result = CSF_AT(int, self, 0x28);
        break;
    case 12:
        result = CSF_AT(int, self, 0x1C);
        break;
    default:
        return result;
    }
    return result;
}

HOOK(0x00744480, MpGameModes_GetConnectionSetting, "auto");
