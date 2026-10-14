#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Locates an entry in the object's 12-byte record array; returns its index or -1.
CSF_FUNC(int, __fastcall, C_00863044_FindEntryIndex, 0x006FE4F0, (void* self, void* edx, int a2));

// Removes the entry matching a2: the last record is moved into the freed slot
// (order is not preserved), then the record count is decremented.
static unsigned int __fastcall C_00863044_EraseEntryCompacting(C_00863044* self, void* /*edx*/, int a2)
{
    unsigned int result = static_cast<unsigned int>(C_00863044_FindEntryIndex(self, nullptr, a2));
    if (result != 0xFFFFFFFFu)
    {
        // Unsigned compare against (count - 1), as in the original.
        if (result < self->field_002c - 1)
        {
            uint32_t base = self->field_0024;
            uint32_t count = self->field_002c;
            uint32_t* lastEntry = reinterpret_cast<uint32_t*>(base + 12 * count - 12);
            uint32_t* slot = reinterpret_cast<uint32_t*>(base + 12 * result);
            slot[0] = lastEntry[0];
            slot[1] = lastEntry[1];
            slot[2] = lastEntry[2];
            // The original leaves the slot address in eax (`result = v4 + 12 * result`
            // in the pseudocode, `lea eax, [ecx + eax*4]` before `ret 4`), so return it.
            result = static_cast<unsigned int>(reinterpret_cast<uintptr_t>(slot));
        }
        --self->field_002c;
    }
    return result;
}

HOOK(0x006FE8F0, C_00863044_EraseEntryCompacting, "auto");
