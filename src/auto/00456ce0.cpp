#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(int, __fastcall, ReleaseObjectReference, 0x006923C0, (void* self, void* edx));

// Releases the two owned references of every entry in the item array, calls
// each entry's slot-0 virtual destructor with flag 1, frees the array, and
// then clears field 0x48 of every entry in the secondary table.
static unsigned int __fastcall C_008312e0_ReleaseItemsAndReset(C_008312e0* self, void* /*edx*/)
{
    uint32_t initialItemCount = self->field_0028;
    uint32_t itemIndex = 0;
    self->field_000c = 0;

    if (initialItemCount != 0) {
        do {
            uint8_t* item = reinterpret_cast<uint8_t*>(
                reinterpret_cast<uint32_t*>(self->field_0020)[itemIndex]);

            uint32_t firstRef = CSF_AT(uint32_t, item, 0x24);
            if (firstRef != 0) {
                ReleaseObjectReference(reinterpret_cast<void*>(firstRef + 4), nullptr);
            }

            uint32_t secondRef = CSF_AT(uint32_t, item, 0x28);
            if (secondRef != 0) {
                ReleaseObjectReference(reinterpret_cast<void*>(secondRef + 4), nullptr);
            }

            ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, item, 0)[0])(item, nullptr, 1);
            ++itemIndex;
        } while (itemIndex < self->field_0028);
    }

    uint32_t itemArray = self->field_0020;
    self->field_0024 = 0;
    self->field_0028 = 0;
    if (itemArray != 0) {
        Mem_Free(reinterpret_cast<void*>(itemArray));
        self->field_0020 = 0;
    }

    uint32_t result = 0;
    for (result = 0; result < self->field_0018; ++result) {
        uint32_t* linkTable = reinterpret_cast<uint32_t*>(self->field_0010);
        uint8_t* linked = reinterpret_cast<uint8_t*>(linkTable[result]);
        CSF_AT(uint32_t, linked, 0x48) = 0;
    }
    return result;
}

HOOK(0x456CE0, C_008312e0_ReleaseItemsAndReset, "auto");
