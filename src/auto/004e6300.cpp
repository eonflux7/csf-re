#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Array of 20-byte elements: +0 data (count prefix at data[-4]), +4 capacity, +8 used count.
static unsigned int __fastcall DynArray20_Grow(void* self, void* /*edx*/, int extra)
{
    int newCapacity = CSF_AT(int, self, 4) + extra;

    uint8_t* newData = nullptr;
    uint8_t* block = static_cast<uint8_t*>(Mem_Alloc(20 * newCapacity + 4));
    if (block) {
        CSF_AT(int, block, 0) = newCapacity;
        newData = block + 4;
        for (int i = 0; i < newCapacity; ++i) {
            uint8_t* elem = newData + i * 20;
            CSF_AT(int, elem, 0) = -1;
            CSF_AT(int, elem, 4) = -1;
            CSF_AT(void**, elem, 8) = CSF_VTBL(0x0082D448);
            CSF_AT(int, elem, 12) = -1;
            CSF_AT(int, elem, 16) = -1;
        }
    }

    unsigned int result = CSF_AT(unsigned int, self, 8);
    if (result) {
        result *= 20;
        std::memcpy(newData, CSF_AT(void*, self, 0), result);
    }

    CSF_AT(int, self, 4) = CSF_AT(int, self, 4) + extra;

    uint8_t* oldData = CSF_AT(uint8_t*, self, 0);
    if (oldData) {
        int oldCount = CSF_AT(int, oldData, -4);
        for (int i = oldCount - 1; i >= 0; --i) {
            CSF_AT(void**, oldData + i * 20, 8) = CSF_VTBL(0x0082D410);
        }
        result = static_cast<unsigned int>(Mem_Free(oldData - 4));
        CSF_AT(uint8_t*, self, 0) = nullptr;
    }

    CSF_AT(uint8_t*, self, 0) = newData;
    return result;
}

HOOK(0x004E6300, DynArray20_Grow, "auto");
