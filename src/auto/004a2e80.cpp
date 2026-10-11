#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Searches the three 16-byte entries at this+0x0C for `key`. If found, the entry's
// index goes into a 3-dword scratch buffer, the sub-object at field_0008+4 gets its
// vtable slot 2 called with that buffer, and the entry's first two dwords are cleared.
// Returns the callee's result, or the number of entries searched (3) when no match.
static int __fastcall C_008344f0_ReleaseEntryMatchingKey(C_008344f0* self, void* /*edx*/, int key)
{
    int result = 0;

    while (key != static_cast<int>(CSF_AT(uint32_t, self, 0x0C + result * 16))) {
        ++result;
        if (result >= 3) {
            return result;
        }
    }

    // Entry found at index `result`.
    const uint32_t entryOffset = 0x0C + static_cast<uint32_t>(result) * 16;

    uint32_t scratch[3];
    scratch[2] = static_cast<uint32_t>(result);
    scratch[0] = 0;
    scratch[1] = 0;

    // Sub-object embedded at field_0008 + 4 (the original loads field_0008, then uses eax+4 as this).
    const uint32_t subobjAddress = self->field_0008 + 4;
    void* subobj = reinterpret_cast<void*>(static_cast<uintptr_t>(subobjAddress));

    using VirtualSlot2Fn = int(__fastcall*)(void* self, void* edx, uint32_t* buffer);
    result = reinterpret_cast<VirtualSlot2Fn>(CSF_AT(void**, subobj, 0)[2])(subobj, nullptr, scratch);

    CSF_AT(uint32_t, self, entryOffset) = 0;
    CSF_AT(uint32_t, self, entryOffset + 4) = 0;

    return result;
}

HOOK(0x004A2E80, C_008344f0_ReleaseEntryMatchingKey, "auto");
