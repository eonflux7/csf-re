#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original: int __thiscall sub_44DE90(S_0044d940 *this, int a2, int a3); ret 8
CSF_FUNC(int, __fastcall, sub_44DBB0, 0x0044DBB0, (void* self, void* edx, int a2, uint32_t* a3));

// Builds a combined key from two records' +0x08 values (smaller * 10000 + larger),
// binary-searches the sorted 12-byte-entry table at this->field_000c (count in field_0014)
// and returns the insertion index. Then forwards the index and key to sub_44DBB0.
static int __fastcall S_0044d940_FindInsertIndexForPairKey(S_0044d940* self, void* /*edx*/, int a2, int a3)
{
    // Reads a3 first, then a2, as the original does.
    uint32_t valueB = CSF_AT(uint32_t, reinterpret_cast<void*>(a3), 8);
    uint32_t valueA = CSF_AT(uint32_t, reinterpret_cast<void*>(a2), 8);

    uint32_t pairKey;
    if (valueA >= valueB)
        pairKey = valueA + 10000u * valueB;
    else
        pairKey = valueB + 10000u * valueA;

    // Stack record handed to sub_44DBB0: key in the first dword, zeros after it.
    alignas(8) uint32_t keyRecord[4] = {pairKey, 0u, 0u, 0u};

    int lowIndex = 0;
    int highIndex = static_cast<int>(self->field_0014) - 1;
    if (highIndex >= 0)
    {
        uintptr_t entryBase = self->field_000c;
        do
        {
            int midIndex = (highIndex + lowIndex) / 2;
            uint32_t midKey = CSF_AT(uint32_t, reinterpret_cast<void*>(entryBase), 12 * midIndex);
            if (pairKey <= midKey)
            {
                if (pairKey >= midKey)
                    break;
                highIndex = midIndex - 1;
            }
            else
            {
                lowIndex = midIndex + 1;
            }
        } while (lowIndex <= highIndex);
    }

    void* tableObject = reinterpret_cast<void*>(reinterpret_cast<uint8_t*>(self) + 0x0C);
    sub_44DBB0(tableObject, nullptr, lowIndex, keyRecord);
    return lowIndex;
}

HOOK(0x0044DE90, S_0044d940_FindInsertIndexForPairKey, "auto");
