#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "core/memory.h"

// Returns the address of a float slot inside a scene entry record.
// Entry records are 0x15c (348) bytes each and start at self->field_0004 + self->field_0010 * 348.
// slotIndex (1..3) selects a block of slots; componentIndex (2/3) adds a per-component offset.
static int __fastcall SceneEntry_GetFloatSlotAddress(void* self, void* /*edx*/, int slotIndex, int componentIndex) {
    int entryBaseAddress = CSF_AT(int, self, 0x04) + CSF_AT(int, self, 0x10) * 0x15c;

    int slotAddress;
    switch (slotIndex) {
        case 1:
            slotAddress = entryBaseAddress + 0x6c;
            break;
        case 2:
            slotAddress = entryBaseAddress + 0xbc;
            break;
        case 3:
            slotAddress = entryBaseAddress + 0x10c;
            break;
        default:
            slotAddress = entryBaseAddress + 0x1c;
            break;
    }

    switch (componentIndex) {
        case 0:
            return slotAddress;
        case 2:
            slotAddress += 0x28;
            break;
        case 3:
            slotAddress += 0x3c;
            break;
        default:
            slotAddress += 0x14;
            break;
    }
    return slotAddress;
}

HOOK(0x00782820, SceneEntry_GetFloatSlotAddress, "auto");
