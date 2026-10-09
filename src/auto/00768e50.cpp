#include <cstdint>
#include "hook.h"
#include "globals.h"

// Returns 1 when the 32-bit key at offset +4 of the first record is below
// the one of the second record, compared unsigned (CF from cmp -> sbb/neg).
static int __cdecl InputBindings_IsFirstKeyBelowSecond(void* first, void* second) {
    uint32_t firstKey = CSF_AT(uint32_t, first, 4);
    uint32_t secondKey = CSF_AT(uint32_t, second, 4);
    return firstKey < secondKey ? 1 : 0;
}

HOOK(0x00768E50, InputBindings_IsFirstKeyBelowSecond, "auto");
