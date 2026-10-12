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

// Copies up to maxCount UTF-16 code units from srcStr to the buffer at
// (destBase - srcStr) + srcStr, i.e. the destination is offset by a constant
// delta computed in 32-bit address arithmetic. Copying stops after a NUL
// code unit has been copied (the NUL check only applies after the first unit).
static void __cdecl CoreVariantValues_CopyWideStringBounded(int destBase, uint16_t* srcStr, int maxCount)
{
    if (destBase == 0 || srcStr == nullptr)
        return;

    // 32-bit address delta, same wrap behavior as the original.
    const uint32_t addressDelta = static_cast<uint32_t>(destBase) - static_cast<uint32_t>(reinterpret_cast<uintptr_t>(srcStr));

    uint16_t* cursor = srcStr;
    int index = 0;
    while ((index == 0 || cursor[-1] != 0) && index < maxCount)
    {
        *reinterpret_cast<uint16_t*>(reinterpret_cast<uint8_t*>(cursor) + addressDelta) = *cursor;
        ++cursor;
        ++index;
    }
}

HOOK(0x6956C0, CoreVariantValues_CopyWideStringBounded, "auto");
