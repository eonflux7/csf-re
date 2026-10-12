#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Reads the 16-bit unit at the current cursor. A nonzero unit advances the cursor
// by one unit (2 bytes); a zero unit (terminator) leaves the cursor in place.
static int16_t __cdecl CoreVariantValues_ReadWcharAndAdvance(int16_t** cursor)
{
    int16_t result = **cursor;
    if (result != 0)
        ++*cursor;
    return result;
}

HOOK(0x00695660, CoreVariantValues_ReadWcharAndAdvance, "auto");
