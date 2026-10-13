#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Binary search over a table of 12-byte records keyed by their leading dword.
// The search key is derived from the +8 dwords of the two pointer arguments:
// the larger value is multiplied by 10000 and the smaller is added to it.
static int __fastcall S_0044d940_FindIndexByPairKey(S_0044d940* self, void* /*edx*/, void* a2, void* a3)
{
    int result = -1;
    uint32_t entryCount = self->field_0014;
    if (entryCount == 0) {
        return result;
    }

    uint32_t valueFromA3 = CSF_AT(uint32_t, a3, 8);
    uint32_t valueFromA2 = CSF_AT(uint32_t, a2, 8);
    uint32_t searchKey;
    if (valueFromA2 >= valueFromA3) {
        searchKey = valueFromA2 + 10000u * valueFromA3;
    } else {
        searchKey = valueFromA3 + 10000u * valueFromA2;
    }

    int lowIndex = 0;
    int highIndex = static_cast<int>(entryCount) - 1;
    if (highIndex >= 0) {
        uint8_t* entryBase = reinterpret_cast<uint8_t*>(self->field_000c);
        do {
            result = (highIndex + lowIndex) / 2;
            uint32_t entryKey = CSF_AT(uint32_t, entryBase, 12 * result);
            if (searchKey <= entryKey) {
                if (searchKey >= entryKey) {
                    return result;
                }
                highIndex = result - 1;
            } else {
                lowIndex = result + 1;
            }
        } while (lowIndex <= highIndex);
    }
    return -1;
}

HOOK(0x0044DA80, S_0044d940_FindIndexByPairKey, "auto");
