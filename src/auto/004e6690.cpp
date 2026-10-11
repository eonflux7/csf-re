#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "types/auto_classes.h"

// Finds the sorted insertion index for an entry key (a2 + 2 dwords in the caller's record).
CSF_FUNC(int, __fastcall, EntryFindInsertIndex, 0x004E5930, (void* self, void* edx, int a2));
// Inserts a 20-byte entry at the given index, shifting the tail (count is already in range).
CSF_FUNC(int, __fastcall, EntryArrayInsertAt, 0x004E6180, (void* self, void* edx, int a2, uint32_t* a3));
// Grows the entry array when count reaches capacity.
CSF_FUNC(unsigned int, __fastcall, EntryArrayGrow, 0x004E6300, (void* self, void* edx, int a2));

static unsigned int __fastcall C_0083e9d0_InsertSortedEntry(C_0083e9d0* self, void* /*edx*/, uint32_t* entry)
{
    uint32_t insertIndex = static_cast<uint32_t>(
        EntryFindInsertIndex(self, nullptr, static_cast<int>(reinterpret_cast<uintptr_t>(entry + 2))));

    if (insertIndex >= self->field_0028)
    {
        // Append at the end: grow first if the array is full.
        if (self->field_0028 >= self->field_0024)
            EntryArrayGrow(&self->field_0020, nullptr, static_cast<int>(CSF_AT(uint32_t, self, 0x2C)));

        uint32_t count = self->field_0028;
        uint32_t entriesBase = self->field_0020;
        self->field_0028 = count + 1;

        uint32_t* slot = reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(entriesBase) + 20u * count);
        slot[0] = entry[0];
        slot[1] = entry[1];
        slot[3] = entry[3];
        slot[4] = entry[4];
        return insertIndex;
    }

    // Insert in the middle: shift existing entries.
    EntryArrayInsertAt(&self->field_0020, nullptr, static_cast<int>(insertIndex), entry);
    return insertIndex;
}

HOOK(0x004E6690, C_0083e9d0_InsertSortedEntry, "auto");
