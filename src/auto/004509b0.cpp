#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"

// Copies four dwords (offsets 4, 8, 16, 20) from the source record a4 into each
// 24-byte record in the range [a2, a3), then returns the pointer just past the last
// written record's +20 field. Stack args: 3, __cdecl (ret 0).
static uint32_t* __cdecl CopyStride24Records(int a2, int a3, void* a4)
{
    uintptr_t cursor = static_cast<uintptr_t>(static_cast<uint32_t>(a2));
    uintptr_t result = cursor + 20;

    if (a2 != a3)
    {
        do
        {
            uint8_t* record = reinterpret_cast<uint8_t*>(cursor);
            CSF_AT(uint32_t, record, 0x04) = CSF_AT(uint32_t, a4, 0x04);
            CSF_AT(uint32_t, record, 0x08) = CSF_AT(uint32_t, a4, 0x08);
            CSF_AT(uint32_t, record, 0x10) = CSF_AT(uint32_t, a4, 0x10);
            CSF_AT(uint32_t, record, 0x14) = CSF_AT(uint32_t, a4, 0x14);
            cursor += 24;
            result += 24;
        } while (cursor != static_cast<uintptr_t>(static_cast<uint32_t>(a3)));
    }

    return reinterpret_cast<uint32_t*>(result);
}

HOOK(0x4509B0, CopyStride24Records, "auto");
