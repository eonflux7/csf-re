#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60, (void* destination, const void* source, std::size_t byteCount));

// Inline block copy (the original uses rep movsd / rep movsb here, not a call)
static void CopyInline(uint8_t* dst, const uint8_t* src, uint32_t byteCount)
{
    for (uint32_t i = 0; i < byteCount; ++i)
        dst[i] = src[i];
}

// Array of 20-byte elements: [0]=data, [4]=capacity, [8]=count, [12]=growBy.
// Inserts one element at 'index', shifting the following elements up.
static int __fastcall ElementArray_Insert(void* self, void* /*edx*/, int index, uint32_t* src)
{
    uint8_t* base = reinterpret_cast<uint8_t*>(self);
    uint32_t count = CSF_AT(uint32_t, base, 8);
    uint32_t capacity = CSF_AT(uint32_t, base, 4);

    if (count < capacity)
    {
        uint8_t* data = CSF_AT(uint8_t*, base, 0);
        _memcpy(data + 20 * index + 20, data + 20 * index, 20 * (count - index));
        uint8_t* elem = CSF_AT(uint8_t*, base, 0) + 20 * index;
        CSF_AT(uint32_t, elem, 0) = src[0];
        CSF_AT(uint32_t, elem, 4) = src[1];
        CSF_AT(uint32_t, elem, 12) = src[3];
        CSF_AT(uint32_t, elem, 16) = src[4];
        int result = CSF_AT(int, base, 8) + 1;
        CSF_AT(int, base, 8) = result;
        return result;
    }

    uint32_t newCapacity = capacity + CSF_AT(uint32_t, base, 12);
    uint8_t* newData = nullptr;
    uint32_t* block = static_cast<uint32_t*>(Mem_Alloc(20 * newCapacity + 4));
    if (block)
    {
        block[0] = newCapacity;
        newData = reinterpret_cast<uint8_t*>(block + 1);
        if (static_cast<int>(newCapacity - 1) >= 0)
        {
            uint8_t* p = newData;
            uint32_t n = newCapacity;
            do
            {
                CSF_AT(uint32_t, p, 0) = 0xFFFFFFFFu;
                CSF_AT(uint32_t, p, 4) = 0xFFFFFFFFu;
                CSF_AT(void*, p, 8) = (void*)CSF_VTBL(0x0082D448);
                CSF_AT(uint32_t, p, 12) = 0xFFFFFFFFu;
                CSF_AT(uint32_t, p, 16) = 0xFFFFFFFFu;
                p += 20;
                --n;
            } while (n);
        }
    }

    uint32_t headBytes = 20 * static_cast<uint32_t>(index);
    CopyInline(newData, CSF_AT(uint8_t*, base, 0), headBytes);

    CSF_AT(uint32_t, newData, headBytes + 0) = src[0];
    CSF_AT(uint32_t, newData, headBytes + 4) = src[1];
    CSF_AT(uint32_t, newData, headBytes + 12) = src[3];
    CSF_AT(uint32_t, newData, headBytes + 16) = src[4];

    uint32_t tailBytes = 20 * (CSF_AT(uint32_t, base, 8) - static_cast<uint32_t>(index));
    CopyInline(newData + headBytes + 20, CSF_AT(uint8_t*, base, 0) + headBytes, tailBytes);

    CSF_AT(uint32_t, base, 4) = CSF_AT(uint32_t, base, 4) + CSF_AT(uint32_t, base, 12);

    uint8_t* oldData = CSF_AT(uint8_t*, base, 0);
    if (oldData)
    {
        int oldCount = CSF_AT(int, oldData, -4);
        uint8_t* oldBlock = oldData - 4;
        uint8_t* p = oldData + 20 * oldCount;
        int remaining = oldCount - 1;
        if (remaining >= 0)
        {
            int n = remaining + 1;
            do
            {
                p -= 20;
                --n;
                CSF_AT(void*, p, 8) = (void*)CSF_VTBL(0x0082D410);
            } while (n);
        }
        Mem_Free(oldBlock);
        CSF_AT(uint32_t, base, 0) = 0;
    }

    int result = CSF_AT(int, base, 8) + 1;
    CSF_AT(uint8_t*, base, 0) = newData;
    CSF_AT(int, base, 8) = result;
    return result;
}

HOOK(0x004E6180, ElementArray_Insert, "auto");
