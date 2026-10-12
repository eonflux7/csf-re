#include <cstddef>
#include <cstring>

#include "globals.h"
#include "hook.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Layout: +0 data pointer (12-byte elements), +4 capacity (element count),
// +8 used element count. Grows the array by extraCount elements, copying the
// used elements into a new block and releasing the old one.
static void* __fastcall RecordArray_Grow(void* self, void* /*edx*/, int extraCount) {
    void* result = Mem_Alloc(static_cast<std::size_t>(12 * (extraCount + CSF_AT(int, self, 4))));
    int usedCount = CSF_AT(int, self, 8);
    if (usedCount != 0) {
        std::memcpy(result, CSF_AT(void*, self, 0), static_cast<std::size_t>(12 * usedCount));
    }

    void* oldBlock = CSF_AT(void*, self, 0);
    CSF_AT(int, self, 4) += extraCount;
    if (oldBlock == nullptr) {
        CSF_AT(void*, self, 0) = result;
    } else {
        result = reinterpret_cast<void*>(Mem_Free(oldBlock));
        CSF_AT(void*, self, 0) = nullptr;
        CSF_AT(void*, self, 0) = reinterpret_cast<void*>(Mem_Free) == nullptr ? result : result;
    }
    return result;
}

HOOK(0x006D9EB0, RecordArray_Grow, "auto");
