#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Returns true when `value` lies in the object's primary range [min, max) (enabled flag at +0x00),
// or in any enabled range record referenced by the pointer array at +0x2C (count at +0x34).
static bool __fastcall RangeList_ContainsValue(void* self, void* /*edx*/, uint32_t value) {
    if (CSF_AT(uint8_t, self, 0x00) != 0 &&
        value >= CSF_AT(uint32_t, self, 0x04) &&
        value < CSF_AT(uint32_t, self, 0x0C)) {
        return true;
    }

    uint32_t entryCount = CSF_AT(uint32_t, self, 0x34);
    if (entryCount == 0) {
        return false;
    }

    uint32_t* entryPointers = reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, self, 0x2C));
    for (uint32_t index = 0; index < entryCount; ++index) {
        void* record = reinterpret_cast<void*>(entryPointers[index]);
        if (CSF_AT(uint8_t, record, 0x00) != 0 &&
            value >= CSF_AT(uint32_t, record, 0x04) &&
            value < CSF_AT(uint32_t, record, 0x0C)) {
            return true;
        }
    }
    return false;
}

HOOK(0x69B320, RangeList_ContainsValue, "auto");
