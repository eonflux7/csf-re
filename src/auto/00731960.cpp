#include <cstdint>
#include "globals.h"
#include "hook.h"

// Clears the active flag (byte at +4) of every non-null entry in a table whose
// stored index (dword at +0xC) differs from `index`. Returns the last entry
// visited, or 0 when the table is empty.
static uint32_t __fastcall ClearEffectsNotMatchingIndex(void* self, void* /*edx*/, int index)
{
    uint32_t count = CSF_AT(uint32_t, self, 0x10);
    uint32_t* entries = CSF_AT(uint32_t*, self, 0x8);
    uint32_t result = 0;

    for (uint32_t i = 0; i < count; ++i)
    {
        result = entries[i];
        if (result)
        {
            uint8_t* effect = reinterpret_cast<uint8_t*>(result);
            if (CSF_AT(int, effect, 0xC) != index)
                CSF_AT(uint8_t, effect, 0x4) = 0;
        }
    }
    return result;
}

HOOK(0x731960, ClearEffectsNotMatchingIndex, "auto");
