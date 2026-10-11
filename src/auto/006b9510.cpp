#include <cstdint>
#include "globals.h"

#include "hook.h"
#include "types/auto_classes.h"

// Original: 0x006B9130, __thiscall with one stack argument; the packet binds it as __fastcall.
CSF_FUNC(int, __fastcall, FindSlotIndex, 0x006B9130, (void* self, void* edx, int a2));

// Each slot is an 8-byte record: [+0] value, [+4] reference count.
// Looks up the slot for a2, bumps its count, and returns its value (0 if not found).
static int __fastcall IncrementSlotCountAndGetValue(S_006b9130* self, void* /*edx*/, int a2)
{
    int slotIndex = FindSlotIndex(self, nullptr, a2);
    if (slotIndex == -1)
        return 0;

    uint8_t* slot = reinterpret_cast<uint8_t*>(self->field_0004) + slotIndex * 8;
    CSF_AT(int, slot, 4) = CSF_AT(int, slot, 4) + 1;
    return CSF_AT(int, slot, 0);
}

HOOK(0x006B9510, IncrementSlotCountAndGetValue, "auto");
