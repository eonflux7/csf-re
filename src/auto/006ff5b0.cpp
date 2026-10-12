#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Stores a2 into the 0x18 field of the entry at index a3 of the table pointed to by
// owner+0x10 (12-byte entries, pointer at +4), but only when the flag bit selected by
// owner+0x20 is set in the bit array reached through owner+0x24 -> +0x64.
static void __fastcall C_008630e0_SetIndexedEntryTargetIfFlagged(C_008630e0* self, void* /*edx*/, int a2, int a3)
{
    uint8_t* owner = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(self->field_0004));
    if (owner == nullptr)
        return;
    if (CSF_AT(uint32_t, owner, 0x24) == 0)
        return;

    uint16_t flagIndex = CSF_AT(uint16_t, owner, 0x20);
    uint8_t* group = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(CSF_AT(uint32_t, owner, 0x24)));
    uint32_t* flagWords = reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(CSF_AT(uint32_t, group, 0x64)));
    uint32_t bitMask = 1u << (flagIndex & 0x1F);

    if ((flagWords[flagIndex >> 5] & bitMask) == 0)
        return;

    uint8_t* entries = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(CSF_AT(uint32_t, owner, 0x10)));
    uint32_t entryPtr = CSF_AT(uint32_t, entries, 12 * a3 + 4);
    if (entryPtr != 0) {
        uint8_t* entry = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(entryPtr));
        CSF_AT(int, entry, 0x18) = a2;
    }
}

HOOK(0x6FF5B0, C_008630e0_SetIndexedEntryTargetIfFlagged, "auto");
