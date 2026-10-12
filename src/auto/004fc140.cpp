#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Returns table[index] when index < table count, otherwise 0.
// Table pointer lives at +0x14, element count at +0x1C (both unsigned compare via jae).
static int __fastcall AnimsGlobalTableGetEntryOrZero(void* self, void* /*edx*/, uint32_t index)
{
    int result = 0;
    uint32_t entryCount = CSF_AT(uint32_t, self, 0x1C);
    if (index < entryCount)
    {
        uint32_t* entries = CSF_AT(uint32_t*, self, 0x14);
        result = static_cast<int>(entries[index]);
    }
    return result;
}

HOOK(0x4FC140, AnimsGlobalTableGetEntryOrZero, "auto");
