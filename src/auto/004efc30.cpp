#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, AiAttackTasks_FindSceneEntryIndex, 0x00770610, (void* self, void* edx, unsigned int a2));
CSF_FUNC(char*, __fastcall, EntryArray_Grow, 0x004EF9E0, (void* self, void* edx, int a2));

// Appends a 32-byte entry {a2, a3, -, a5, a6, -1, 0, 0} to the dynamic array at this+0x1c
// (data ptr +0x1c, capacity +0x20, count +0x24, grow step +0x28), if a3 is found in the scene.
// Returns the new entry's address, or -1 if the scene lookup failed.
static int __fastcall S_004ef780_AppendEntryWithCategory(S_004ef780* self, void* /*edx*/,
                                                         int a2, uint32_t a3, int a4,
                                                         int a5, int a6) {
    (void)a4;  // not used by the original

    int result = AiAttackTasks_FindSceneEntryIndex(self, nullptr, static_cast<unsigned int>(a3));
    if (result == -1) {
        return result;
    }

    uint32_t count = self->field_0024;
    uint32_t capacity = self->field_0020;
    if (count >= capacity) {
        uint32_t growStep = CSF_AT(uint32_t, self, 0x28);
        EntryArray_Grow(reinterpret_cast<uint8_t*>(self) + 0x1c, nullptr, static_cast<int>(growStep));
    }

    // Re-read after the grow call: the array may have changed.
    count = self->field_0024;
    uint32_t dataBase = self->field_001c;
    uint32_t entryAddress = dataBase + 32u * count;
    self->field_0024 = count + 1;

    result = static_cast<int>(entryAddress);
    uint8_t* entry = reinterpret_cast<uint8_t*>(entryAddress);
    CSF_AT(int, entry, 0x00) = a2;
    CSF_AT(uint32_t, entry, 0x04) = a3;
    CSF_AT(int, entry, 0x0C) = a5;
    CSF_AT(int, entry, 0x10) = a6;
    CSF_AT(int, entry, 0x14) = -1;
    CSF_AT(uint8_t, entry, 0x18) = 0;
    CSF_AT(int, entry, 0x1C) = 0;

    return result;
}

HOOK(0x004EFC30, S_004ef780_AppendEntryWithCategory, "auto");
