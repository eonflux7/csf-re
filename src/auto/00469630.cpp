#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, S_0045d610_ProcessEntry, 0x00469190, (void* self, void* edx, int a2));

// Walks the entry array (12-byte records starting at field_0084), calls the per-entry
// handler for each, then clears the entry count. Returns the last handler's result,
// or the original count when it is zero.
static unsigned int __fastcall S_0045d610_ProcessEntriesAndReset(S_0045d610* self, void* /*edx*/)
{
    unsigned int result = self->field_008c;
    if (result)
    {
        int offset = 0;
        uint32_t remaining = self->field_008c;
        do
        {
            result = S_0045d610_ProcessEntry(self, nullptr, (int)(self->field_0084 + offset));
            offset += 12;
            --remaining;
        } while (remaining);
        self->field_008c = 0;
    }
    return result;
}

HOOK(0x00469630, S_0045d610_ProcessEntriesAndReset, "auto");
