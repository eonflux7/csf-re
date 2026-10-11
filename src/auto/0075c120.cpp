#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Layout: +0 data (12-byte elements), +4 capacity, +8 count, +12 extra value.
static void* __fastcall WaypointGraph_AssignElementArray(void* self, void* /*edx*/, void* other)
{
    uint8_t* dst = static_cast<uint8_t*>(self);
    uint8_t* src = static_cast<uint8_t*>(other);

    if (CSF_AT(uint32_t, src, 8) > CSF_AT(uint32_t, dst, 4)) {
        int capacity = CSF_AT(int, src, 4);
        int* block = static_cast<int*>(Mem_Alloc(12 * capacity));
        int* newData;
        if (block) {
            int last = capacity - 1;
            if (last >= 0) {
                int remaining = last + 1;
                int* p = block;
                do {
                    p[0] = -1;
                    p[1] = -1;
                    p[2] = -1;
                    p += 3;
                    --remaining;
                } while (remaining);
            }
            newData = block;
        } else {
            newData = nullptr;
        }

        void* oldData = CSF_AT(void*, dst, 0);
        CSF_AT(int, dst, 4) = CSF_AT(int, src, 4);
        if (oldData != nullptr) {
            Mem_Free(oldData);
            CSF_AT(void*, dst, 0) = nullptr;
        }
        CSF_AT(void*, dst, 0) = newData;
    }

    uint32_t count = CSF_AT(uint32_t, src, 8);
    if (count != 0) {
        std::memcpy(CSF_AT(void*, dst, 0), CSF_AT(void*, src, 0), count * 12);
    }
    CSF_AT(int, dst, 8) = CSF_AT(int, src, 8);
    CSF_AT(int, dst, 12) = CSF_AT(int, src, 12);
    return self;
}

HOOK(0x0075C120, WaypointGraph_AssignElementArray, "auto");
