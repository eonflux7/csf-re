#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Scans the 28-byte record array at self+0x198 (count at self+0x1a0) and, for the
// first record with a non-negative id at +8 whose +4 field equals key+4, marks the
// record as free (+4 and +8 set to -1) and clears bit 1 of its +0x18 flags.
// Returns the 1-based index of the last record visited.
static unsigned int __fastcall ClearSlotMatchingKey(void* self, void* /*edx*/, void* key) {
    uint32_t recordCount = CSF_AT(uint32_t, self, 0x1a0);
    unsigned int result = recordCount;
    uint32_t recordIndex = 0;
    uint32_t byteOffset = 0;
    bool matched = false;

    if (recordCount != 0) {
        do {
            if (matched) {
                break;
            }
            uint8_t* records = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x198));
            int32_t recordId = CSF_AT(int32_t, records, byteOffset + 8);

            if (recordId >= 0 && CSF_AT(int32_t, records, byteOffset + 4) == CSF_AT(int32_t, key, 4)) {
                CSF_AT(int32_t, records, byteOffset + 4) = -1;
                CSF_AT(int32_t, records, byteOffset + 8) = -1;
                uint8_t* flagsBase = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x198));
                CSF_AT(uint32_t, flagsBase, byteOffset + 0x18) &= ~2u;
                matched = true;
            }

            result = recordIndex + 1;
            byteOffset += 28;
            ++recordIndex;
        } while (recordIndex < CSF_AT(uint32_t, self, 0x1a0));
    }
    return result;
}

HOOK(0x49F1A0, ClearSlotMatchingKey, "auto");
