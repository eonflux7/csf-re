#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Replaces the pair table owned by 'self' with a zeroed table sized from the source
// object, then copies the source's pair data and two trailing values.
static int __fastcall S_006b0320_AssignPairTable(S_006b0320* self, void* /*edx*/, int a2)
{
    auto* src = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(a2));

    if (CSF_AT(uint32_t, src, 8) > self->field_0004)
    {
        int32_t pairCount = static_cast<int32_t>(CSF_AT(uint32_t, src, 4));
        void* block = Mem_Alloc(8 * static_cast<std::size_t>(pairCount));

        uint32_t newTable = 0;
        if (block)
        {
            if (pairCount - 1 >= 0)
            {
                auto* cursor = static_cast<uint32_t*>(block);
                for (int32_t i = 0; i < pairCount; ++i)
                {
                    cursor[0] = 0;
                    cursor[1] = 0;
                    cursor += 2;
                }
            }
            newTable = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(block));
        }

        uint32_t oldTable = self->field_0000;
        self->field_0004 = CSF_AT(uint32_t, src, 4);
        if (oldTable != 0)
        {
            Mem_Free(reinterpret_cast<void*>(static_cast<uintptr_t>(oldTable)));
            self->field_0000 = 0;
        }
        self->field_0000 = newTable;
    }

    uint32_t copyCount = CSF_AT(uint32_t, src, 8);
    if (copyCount != 0)
    {
        std::memcpy(
            reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0000)),
            reinterpret_cast<const void*>(static_cast<uintptr_t>(CSF_AT(uint32_t, src, 0))),
            static_cast<std::size_t>(copyCount) * 8);
    }

    self->field_0008 = CSF_AT(uint32_t, src, 8);
    self->field_000c = CSF_AT(uint32_t, src, 12);
    return static_cast<int>(reinterpret_cast<uintptr_t>(self));
}

HOOK(0x006B0320, S_006b0320_AssignPairTable, "auto");
