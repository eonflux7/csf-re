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

// Grows a dynamic array of 12-byte entries (three dwords, initialised to -1).
// Layout: +0 data pointer, +4 capacity, +8 used count.
static const void* __fastcall GrowControlBindingArray(void* self, void* /*edx*/, int growBy)
{
    uint8_t* base = reinterpret_cast<uint8_t*>(self);

    int newCapacity = growBy + CSF_AT(int, base, 4);
    int32_t* newData = static_cast<int32_t*>(Mem_Alloc(static_cast<std::size_t>(newCapacity) * 12));

    if (newData) {
        int last = newCapacity - 1;
        if (last >= 0) {
            int32_t* p = newData;
            for (int n = last + 1; n != 0; --n) {
                p[0] = -1;
                p[1] = -1;
                p[2] = -1;
                p += 3;
            }
        }
    }

    uint32_t usedCount = CSF_AT(uint32_t, base, 8);
    if (usedCount != 0) {
        std::memcpy(newData, CSF_AT(void*, base, 0), usedCount * 12);
    }

    const void* result = CSF_AT(void*, base, 0);
    CSF_AT(int, base, 4) = CSF_AT(int, base, 4) + growBy;

    if (CSF_AT(void*, base, 0) != nullptr) {
        result = reinterpret_cast<const void*>(static_cast<intptr_t>(Mem_Free(CSF_AT(void*, base, 0))));
        CSF_AT(void*, base, 0) = nullptr;
    }
    CSF_AT(void*, base, 0) = newData;
    return result;
}

HOOK(0x0075C1C0, GrowControlBindingArray, "auto");
