#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// sub_439C10: __thiscall, no stack args, returns unsigned int.
CSF_FUNC(unsigned int, __fastcall, sub_439C10, 0x00439C10, (void* self, void* edx));

// sub_438880: __thiscall, 4 stack args (ret 16).
CSF_FUNC(void, __fastcall, sub_438880, 0x00438880, (void* self, void* edx, int a3, float* a4, int a5, int a6));

// Flushes the object referenced at +0x30 through its vtable slot 9, runs the
// base-step routine sub_439C10, then walks the 32-byte records at the pointer
// stored at +0x34 (count at +0x3C), handing each record to sub_438880.
static int __fastcall S_00439ba0_FlushAndProcessRecords(S_00439ba0* self, void* /*edx*/)
{
    // Virtual call on the object at +0x30: vtable slot 36 / 4 = 9.
    void* flushTarget = reinterpret_cast<void*>(self->field_0030);
    void** flushVtbl = CSF_AT(void**, flushTarget, 0);
    ((void(__fastcall*)(void*, void*))flushVtbl[9])(flushTarget, nullptr);

    sub_439C10(self, nullptr);

    uint32_t recordCount = self->field_003c;
    if (recordCount != 0)
    {
        uint32_t byteOffset = 0;
        uint32_t remaining = recordCount;
        do
        {
            // Record base pointer is re-read from +0x34 each iteration.
            uint8_t* recordBase = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x34));
            float v4[8];
            std::memcpy(v4, recordBase + byteOffset, sizeof(v4));

            // +0x30 is re-read per call, as in the original.
            sub_438880(self, nullptr,
                       (int)self->field_0030,
                       v4,
                       (int)(uintptr_t)(&v4[4]),
                       (int)(uintptr_t)(&v4[6]));

            byteOffset += 32;
            --remaining;
        } while (remaining != 0);
    }

    // Original leaves eax = recordCount - 1 at the last iteration, which is 0,
    // or the untouched count (0) when the loop was skipped.
    return 0;
}

HOOK(0x00439C80, S_00439ba0_FlushAndProcessRecords, "auto");
