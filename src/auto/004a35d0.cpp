#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, FindEntrySlotIndex, 0x00771B80, (void* self, void* edx, int a2));

static int __fastcall C_008344f0_FindSlotIndexForObject(C_008344f0* self, void* /*edx*/, S_00771b80* a2)
{
    // Entry table: 12-byte records [ +0 unknown, +4 S_00771b80* key, +8 int slot index ].
    int foundValue = -1;
    uint32_t entryCount = 0;
    if (self->field_006c != 0)
        entryCount = static_cast<uint32_t>(static_cast<int>(self->field_0070 - self->field_006c) / 12);

    bool matched = false;
    if (entryCount != 0) {
        uint8_t* table = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(self->field_006c));
        uint32_t index = 0;
        while (true) {
            S_00771b80* key = *reinterpret_cast<S_00771b80**>(table + 12 * index + 4);
            if (key == a2) {
                matched = true;
                break;
            }
            ++index;
            if (index >= entryCount)
                break;
        }
        if (matched) {
            foundValue = *reinterpret_cast<int*>(table + 12 * index + 8);
        }
    }

    if (matched && foundValue != -1)
        return foundValue;

    // Fallback: look the object up in the owner's secondary table.
    uint8_t* owner = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(self->field_0008));
    uint32_t ownerTable = CSF_AT(uint32_t, owner, 0x2C);
    uint8_t* secondary = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(ownerTable));

    (void)CSF_VTBL(0x00771B80);
    int slotIndex = FindEntrySlotIndex(secondary + 0xF0, nullptr, static_cast<int>(reinterpret_cast<uintptr_t>(a2)));
    if (slotIndex != -1) {
        uint8_t* slotBase = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(CSF_AT(uint32_t, secondary, 0xF4)));
        return *reinterpret_cast<int*>(slotBase + 12 * slotIndex + 8);
    }

    return foundValue;
}

HOOK(0x004A35D0, C_008344f0_FindSlotIndexForObject, "auto");
