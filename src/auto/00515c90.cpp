#include <cstdint>
#include "globals.h"
#include <cstddef>

#include "hook.h"
#include "types/auto_classes.h"

// Finds the 80-byte entry in C_00840af8's array whose key field (at entry+0x48)
// equals `key`. Returns the entry's address, or the first entry's address (begin)
// if no entry matches (index stays 0 when the array is empty or the scan misses).
static int __fastcall C_00840af8_FindEntryByKey(C_00840af8* self, void* /*edx*/, int key) {
    uint32_t entryCount = 0;
    if (self->field_0008 != 0) {
        // Signed element count: (end - begin) / sizeof(entry), sizeof(entry) = 80.
        int byteSpan = static_cast<int>(self->field_000c - self->field_0008);
        entryCount = static_cast<uint32_t>(byteSpan / 80);
    }

    uint32_t entryIndex = 0;
    if (entryCount != 0) {
        uint8_t* entry = reinterpret_cast<uint8_t*>(self->field_0008) + 72;
        do {
            if (CSF_AT(int, entry, 0) == key) {
                break;
            }
            ++entryIndex;
            entry += 80;
        } while (entryIndex < entryCount);
    }

    return static_cast<int>(self->field_0008 + 80 * entryIndex);
}

HOOK(0x515C90, C_00840af8_FindEntryByKey, "auto");
