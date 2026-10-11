#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Clears the byte at +0x20 of every non-null entry in the table at this+0x4C.
// The count comes from the object at this+0x14 (dword at +4). Returns the last
// value read from the table, or the header pointer if the loop never ran.
static int __fastcall C_00861e98_ClearAllEntryFlags(C_00861e98* self, void* /*edx*/)
{
    uint32_t result = CSF_AT(uint32_t, self, 0x14);
    int count = CSF_AT(int, reinterpret_cast<void*>(result), 4);

    for (int i = 0; i < count; ++i)
    {
        uint32_t* table = reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, self, 0x4C));
        result = table[i];
        if (result != 0)
        {
            CSF_AT(uint8_t, reinterpret_cast<void*>(result), 0x20) = 0;
        }
    }

    return static_cast<int>(result);
}

HOOK(0x6E8BC0, C_00861e98_ClearAllEntryFlags, "auto");
