#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Returns either the address of the inline payload slot of an indexed entry
// (when the entry's count at +0x34 is below 16), or the int stored in that slot
// (when the count is 16 or more). Returns 0 when the owner's flag at +0x96 is clear.
static int __fastcall GetSceneEntrySlotValue(void* self, void* /*edx*/, int index)
{
    if (CSF_AT(uint8_t, self, 0x96) == 0)
        return 0;

    uint32_t* entryTable = CSF_AT(uint32_t*, self, 0xA0);
    uint8_t* entry = reinterpret_cast<uint8_t*>(entryTable[index]);
    uint32_t count = CSF_AT(uint32_t, entry, 0x34);

    uint8_t* payloadSlot = entry + 0x20;
    if (count < 0x10)
        return static_cast<int>(reinterpret_cast<uintptr_t>(payloadSlot));

    return CSF_AT(int, entry, 0x20);
}

HOOK(0x6CC7F0, GetSceneEntrySlotValue, "auto");
