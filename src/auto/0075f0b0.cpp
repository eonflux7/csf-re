#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Grows a vector-like container of 36-byte elements by `extra` slots:
//   +0 data pointer, +4 capacity, +8 element count.
static int __fastcall WaypointGraph_GrowElementArray(void* self, void* /*edx*/, int extra)
{
    uint8_t* base = static_cast<uint8_t*>(self);

    int newCapacity = CSF_AT(int, base, 4) + extra;
    uint8_t* newData = static_cast<uint8_t*>(Mem_Alloc(static_cast<std::size_t>(36 * newCapacity)));
    int result = 0;

    if (newData) {
        int last = newCapacity - 1;
        if (last >= 0) {
            int remaining = last + 1;
            uint8_t* elem = newData;
            do {
                CSF_AT(int, elem, 4) = -1;
                CSF_AT(int, elem, 8) = -1;
                CSF_AT(int, elem, 0xC) = 0;
                CSF_AT(int, elem, 0x10) = 0;
                CSF_AT(int, elem, 0x14) = 0;
                elem += 0x24;
                --remaining;
            } while (remaining);
        }
    }

    uint32_t count = CSF_AT(uint32_t, base, 8);
    if (count > 0) {
        std::memcpy(newData, CSF_AT(void*, base, 0), 36 * count);
        result = 0;
    }

    void* oldData = CSF_AT(void*, base, 0);
    CSF_AT(int, base, 4) += extra;
    if (oldData != nullptr) {
        result = Mem_Free(oldData);
        CSF_AT(void*, base, 0) = nullptr;
    }
    CSF_AT(void*, base, 0) = newData;
    return result;
}

HOOK(0x0075F0B0, WaypointGraph_GrowElementArray, "auto");
