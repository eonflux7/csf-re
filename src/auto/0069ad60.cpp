#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

// Frees the two parallel pointer tables (slots 0..count-1 and 15..(count+14) at +0x3C)
// and resets the entry count to zero. Returns the count as last loaded in the loop.
static unsigned int __fastcall S_0069ad60_ReleaseEntries(S_0069ad60* self, void* /*edx*/)
{
    unsigned int result = self->field_007c;
    for (unsigned int i = 0; i < result; ++i)
    {
        uint32_t primaryPtr = CSF_AT(uint32_t, self, 4 * i);
        if (primaryPtr != 0)
        {
            CoreMemory_Free(reinterpret_cast<void*>(primaryPtr));
            CSF_AT(uint32_t, self, 4 * i) = 0;
        }

        uint32_t secondaryPtr = CSF_AT(uint32_t, self, 0x3C + 4 * i);
        if (secondaryPtr != 0)
        {
            CoreMemory_Free(reinterpret_cast<void*>(secondaryPtr));
            CSF_AT(uint32_t, self, 0x3C + 4 * i) = 0;
        }

        result = self->field_007c;
    }
    self->field_007c = 0;
    return result;
}

HOOK(0x0069AD60, S_0069ad60_ReleaseEntries, "auto");
