#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Each slot entry in CNetProfile is 0x24C (588) bytes; the slot table starts at offset 0.
// Offsets used here:
//   +0x1704  dword  last result code (0 = ok, 1 = busy, 12 = index out of range)
//   +0x1708  byte   busy/lock flag
//   slot + 0x14  byte  slot-in-use flag
//   slot + 0x0C  first field of the slot payload (returned to caller)
static void* __fastcall CNetProfile_GetSlotPayloadIfReady(void* self, void* /*edx*/, int slotIndex)
{
    if (slotIndex < 0 || slotIndex >= 10)
    {
        CSF_AT(int, self, 0x1704) = 12;
        return nullptr;
    }

    const uint8_t busyFlag = CSF_AT(uint8_t, self, 0x1708);
    CSF_AT(int, self, 0x1704) = 0;

    if (busyFlag != 0)
    {
        CSF_AT(int, self, 0x1704) = 1;
        return nullptr;
    }

    uint8_t* slotEntry = static_cast<uint8_t*>(self) + slotIndex * 0x24C;
    if (CSF_AT(uint8_t, slotEntry, 0x14) != 0)
    {
        return slotEntry + 0x0C;
    }

    return nullptr;
}

HOOK(0x5177A0, CNetProfile_GetSlotPayloadIfReady, "auto");
