#include <cstdint>
#include <cstddef>
#include "hook.h"

// Returns the address of a per-slot field inside a sound object (offsets 0x30..0x5C),
// or nullptr for unmapped slot ids. Slots 69..88 share the field at 0x5C.
static char* __fastcall GetSoundParamSlotAddress(char* self, void* /*edx*/, int slot) {
    char* result = nullptr;
    switch (slot) {
    case 9:
        result = self + 0x30;
        break;
    case 10:
        result = self + 0x34;
        break;
    case 14:
        result = self + 0x50;
        break;
    case 15:
        result = self + 0x54;
        break;
    case 16:
        result = self + 0x58;
        break;
    case 56:
        result = self + 0x38;
        break;
    case 57:
        result = self + 0x3C;
        break;
    case 58:
        result = self + 0x40;
        break;
    case 59:
        result = self + 0x44;
        break;
    case 60:
        result = self + 0x48;
        break;
    case 61:
        result = self + 0x4C;
        break;
    default:
        if (slot >= 69 && slot <= 88) {
            result = self + 0x5C;
        }
        break;
    }
    return result;
}

HOOK(0x525270, GetSoundParamSlotAddress, "auto");
