#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Releases a heap block holding a counted array of 28-byte records.
// Layout: [count:int32] at block-4, records start at block.
// Each record's dword at +4 is overwritten with C_0082d410's vtable,
// iterating from the last record down to the first, then the block is freed
// and the array pointer is cleared.
static void __fastcall ReleaseObjectArray(int* self, void* /*edx*/)
{
    CSF_AT(int, self, 4) = 0;
    CSF_AT(int, self, 8) = 0;

    if (CSF_AT(int, self, 0) != 0)
    {
        uint8_t* arrayData = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(CSF_AT(int, self, 0)));
        int elementCount = CSF_AT(int, arrayData, -4);

        uint8_t* slot = arrayData + 28 * elementCount + 4;
        for (int remaining = elementCount; remaining > 0; --remaining)
        {
            slot -= 28;
            *reinterpret_cast<void***>(slot) = CSF_VTBL(0x0082D410);
        }

        Mem_Free(static_cast<void*>(arrayData - 4));
        CSF_AT(int, self, 0) = 0;
    }
}

HOOK(0x004DC810, ReleaseObjectArray, "auto");
