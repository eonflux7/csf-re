#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Returns the player id stored in slot `slot` of the object, or -1 when slot is out of range 0..15.
// Slot 0 lives at +0x04 (field_0004); slots 1..15 live at +0x08 .. +0x40 in 4-byte steps.
static int __fastcall S_00749b70_GetPlayerIdBySlot(S_00749b70* self, void* /*edx*/, int slot) {
    int result = -1;
    switch (slot) {
        case 0:
            result = (int)self->field_0004;
            break;
        case 1:
            result = CSF_AT(int, self, 0x08);
            break;
        case 2:
            result = CSF_AT(int, self, 0x0C);
            break;
        case 3:
            result = CSF_AT(int, self, 0x10);
            break;
        case 4:
            result = CSF_AT(int, self, 0x14);
            break;
        case 5:
            result = CSF_AT(int, self, 0x18);
            break;
        case 6:
            result = CSF_AT(int, self, 0x1C);
            break;
        case 7:
            result = CSF_AT(int, self, 0x20);
            break;
        case 8:
            result = CSF_AT(int, self, 0x24);
            break;
        case 9:
            result = CSF_AT(int, self, 0x28);
            break;
        case 10:
            result = CSF_AT(int, self, 0x2C);
            break;
        case 11:
            result = CSF_AT(int, self, 0x30);
            break;
        case 12:
            result = CSF_AT(int, self, 0x34);
            break;
        case 13:
            result = CSF_AT(int, self, 0x38);
            break;
        case 14:
            result = CSF_AT(int, self, 0x3C);
            break;
        case 15:
            result = CSF_AT(int, self, 0x40);
            break;
        default:
            return result;
    }
    return result;
}

HOOK(0x00749B70, S_00749b70_GetPlayerIdBySlot, "auto");
