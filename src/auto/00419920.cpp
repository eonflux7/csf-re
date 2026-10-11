#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Growable array of 28-byte elements: [0]=data, [4]=capacity, [8]=count.
// Grows capacity by `extra` elements, copying existing elements to the new buffer.
static int __fastcall VehicleArray_Grow(void* self, void* /*edx*/, int extra)
{
    int newCapacity = CSF_AT(int, self, 4) + extra;
    uint32_t* block = static_cast<uint32_t*>(Mem_Alloc(28 * newCapacity));
    void* newData;

    if (block) {
        int remaining = newCapacity - 1;
        if (remaining >= 0) {
            uint32_t* p = block;
            int n = remaining + 1;
            do {
                p[0] = 0;
                p[1] = 0;
                p[2] = 0;
                p[3] = 0;
                p[4] = 0;
                p[5] = 0;
                p += 7;
                --n;
            } while (n != 0);
        }
        newData = block;
    } else {
        newData = nullptr;
    }

    uint32_t count = CSF_AT(uint32_t, self, 8);
    if (count > 0) {
        std::memcpy(newData, CSF_AT(void*, self, 0), 28 * count);
    }

    void* oldData = CSF_AT(void*, self, 0);
    CSF_AT(int, self, 4) = CSF_AT(int, self, 4) + extra;

    int result;
    if (oldData == nullptr) {
        result = static_cast<int>(reinterpret_cast<uintptr_t>(newData));
    } else {
        result = Mem_Free(oldData);
        CSF_AT(void*, self, 0) = nullptr;
    }
    CSF_AT(void*, self, 0) = newData;
    return result;
}

HOOK(0x00419920, VehicleArray_Grow, "auto");
