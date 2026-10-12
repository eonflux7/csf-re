#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "core/memory.h"

// Returns the index of the first entry in the region table whose
// key (entry->field at +4) equals a2, or -1 when no entry matches.
static int __fastcall SceneLoader_ResolveRegionIndex(void* self, void* /*edx*/, int regionKey)
{
    const uint32_t entryCount = CSF_AT(uint32_t, self, 0xC);
    int result = -1;
    uint32_t index = 0;

    if (entryCount != 0)
    {
        void* const* entries = reinterpret_cast<void* const*>(CSF_AT(uint32_t, self, 4));
        for (;;)
        {
            void* entry = entries[index];
            if (regionKey == CSF_AT(int, entry, 4))
            {
                return static_cast<int>(index);
            }
            ++index;
            if (index >= entryCount)
            {
                return result;
            }
        }
    }
    return result;
}

HOOK(0x00774C30, SceneLoader_ResolveRegionIndex, "auto");
