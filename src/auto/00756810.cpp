#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Reads the entry at table[index], where table is the pointer stored at self+0x08.
// Only called from the cases that the original dereferences.
static int ReadStateSlotEntry(void* self, int index)
{
    const uint32_t tableAddress = CSF_AT(uint32_t, self, 0x08);
    const int* table = reinterpret_cast<const int*>(tableAddress);
    return table[index];
}

// __thiscall int ResolveSlotIndexForState(this, int slotIndex, int stateCode), ret 8
static int __fastcall ResolveSlotIndexForState(void* self, void* /*edx*/, int slotIndex, int stateCode)
{
    if (slotIndex >= 4)
        return -1;

    switch (stateCode)
    {
    case 4:
        return ReadStateSlotEntry(self, slotIndex + 41);
    case 5:
        return ReadStateSlotEntry(self, slotIndex + 33);
    case 6:
    case 7:
        return ReadStateSlotEntry(self, slotIndex + 37);
    case 25:
        return ReadStateSlotEntry(self, slotIndex + 48);
    case 30:
        return ReadStateSlotEntry(self, slotIndex + 52);
    default:
        return -1;
    }
}

HOOK(0x756810, ResolveSlotIndexForState, "auto");
