#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Copies a zero-terminated UTF-16 string from a1 into a2. If a1 is empty, writes a
// single terminator to a2 and returns a2; otherwise returns a pointer to the terminator
// of a1 (the original's `result` cursor).
static uint8_t* __cdecl CoreZlibLog_ConvertAttributeTextToWide(uint16_t* a1, uint16_t* a2, int /*a3*/) {
    uint8_t* result = reinterpret_cast<uint8_t*>(a1);

    if (*a1 != 0) {
        uint16_t* dst = a2;
        do {
            // Original tests only the low byte here (cmp byte ptr [eax], 0).
            if (*result == 0)
                break;
            *dst = *reinterpret_cast<uint16_t*>(result);
            result += 2;
            ++dst;
        } while (*reinterpret_cast<uint16_t*>(result) != 0);
        *dst = 0;
        return result;
    }

    *a2 = 0;
    return reinterpret_cast<uint8_t*>(a2);
}

HOOK(0x006955E0, CoreZlibLog_ConvertAttributeTextToWide, "auto");
