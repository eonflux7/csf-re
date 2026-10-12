#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Copies the DWORDs in [rangeBegin, rangeEnd) into the slots ending at destEnd,
// walking backward from the last element. Returns the new (lowest) destination slot.
static uint32_t* __cdecl VariantValues_CopyDwordRangeReverse(uint32_t rangeBegin, uint32_t rangeEnd, uint32_t* destEnd) {
    uint32_t cursor = rangeEnd;
    uint32_t* result = destEnd;
    while (cursor != rangeBegin) {
        uint32_t value = *reinterpret_cast<uint32_t*>(cursor - 4);
        cursor -= 4;
        --result;
        *result = value;
    }
    return result;
}

HOOK(0x6958C0, VariantValues_CopyDwordRangeReverse, "auto");
