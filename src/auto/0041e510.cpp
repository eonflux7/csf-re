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

// Dynamic array of 32-bit values: data(+0), capacity(+4), count(+8), growBy(+0xC).
// Inserts *value at index, shifting following elements up.
static int __fastcall DynArray_InsertAt(void* self, void* /*edx*/, unsigned int index, uint32_t* value)
{
    uint32_t count = CSF_AT(uint32_t, self, 8);
    uint32_t capacity = CSF_AT(uint32_t, self, 4);

    if (count >= capacity) {
        uint32_t* newData = static_cast<uint32_t*>(Mem_Alloc(4 * (capacity + CSF_AT(uint32_t, self, 0xC))));
        uint32_t* oldData = CSF_AT(uint32_t*, self, 0);

        uint32_t headCount = index;
        for (uint32_t i = 0; i < headCount; ++i)
            newData[i] = oldData[i];

        newData[index] = *value;

        uint32_t tailCount = CSF_AT(uint32_t, self, 8) - index;
        uint32_t* src = CSF_AT(uint32_t*, self, 0) + index;
        uint32_t* dst = newData + index + 1;
        for (uint32_t i = 0; i < tailCount; ++i)
            dst[i] = src[i];

        void* oldBlock = CSF_AT(void*, self, 0);
        CSF_AT(uint32_t, self, 4) += CSF_AT(uint32_t, self, 0xC);
        if (oldBlock != nullptr) {
            Mem_Free(oldBlock);
            CSF_AT(void*, self, 0) = nullptr;
        }
        CSF_AT(uint32_t*, self, 0) = newData;
        int result = static_cast<int>(CSF_AT(uint32_t, self, 8)) + 1;
        CSF_AT(uint32_t, self, 8) = result;
        return result;
    }

    uint32_t* data = CSF_AT(uint32_t*, self, 0);
    _memcpy(data + index + 1, data + index, (count - index) * 4);
    CSF_AT(uint32_t*, self, 0)[index] = *value;
    int result = static_cast<int>(CSF_AT(uint32_t, self, 8)) + 1;
    CSF_AT(uint32_t, self, 8) = result;
    return result;
}

HOOK(0x0041E510, DynArray_InsertAt, "auto");
