#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "types/auto_classes.h"

// Walks an array of 56-byte entries (stride 0x38). Each entry's +4 pointer is a
// polymorphic object (or null, in which case the struct's +0x1C default object is
// used). Its virtual slot 6 (byte offset 0x18), called with arg 1, yields an int
// weight. The running float sum of those weights is compared against the threshold,
// and the index of the first entry where the sum exceeds it is returned (-1 if none).
static int __fastcall S_00507790_FindCumulativeThresholdIndex(S_00507790* self, void* /*edx*/, float threshold)
{
    uint32_t entryCount = self->field_005c;
    float cumulativeWeight = 0.0f;
    int selectedIndex = -1;
    uint32_t entryIndex = 0;
    uint32_t entryOffset = 0;

    while (selectedIndex == -1) {
        if (entryIndex >= entryCount) {
            break;
        }

        uint32_t entryObject = CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(self->field_0054 + entryOffset), 4);
        if (entryObject == 0) {
            entryObject = self->field_001c;
        }

        void* weightSource = reinterpret_cast<void*>(entryObject);
        int entryWeight = ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, weightSource, 0)[6])(weightSource, nullptr, 1);

        cumulativeWeight = static_cast<float>(entryWeight) + cumulativeWeight;
        if (cumulativeWeight > threshold) {
            selectedIndex = static_cast<int>(entryIndex);
        }

        ++entryIndex;
        entryOffset += 0x38;
    }

    return selectedIndex;
}

HOOK(0x507790, S_00507790_FindCumulativeThresholdIndex, "auto");
