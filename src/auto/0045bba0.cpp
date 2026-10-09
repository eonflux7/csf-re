#include <cstddef>
#include <cstdint>
#include <cstring>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Grows a buffer of 8-byte entries (pairs of ints) by extraCount entries,
// fills the new capacity with -1 pairs, copies the used entries over, and
// frees the old block. Returns the old block pointer (as the original does).
static const void* __fastcall ResizePairBuffer(void* self, void* /*edx*/, int extraCount)
{
    int* capacityField = CSF_AT(int*, self, 0x04);   // this+4
    int* usedField = CSF_AT(int*, self, 0x08);       // this+8
    void** dataField = CSF_AT(void**, self, 0x00);   // this+0

    int newCapacity = extraCount + *capacityField;

    void* newBuffer = nullptr;
    int* allocated = static_cast<int*>(Mem_Alloc(static_cast<std::size_t>(8 * newCapacity)));
    if (allocated)
    {
        int* cursor = allocated;
        if (newCapacity - 1 >= 0)
        {
            int remaining = newCapacity;
            do
            {
                cursor[0] = -1;
                cursor[1] = -1;
                cursor += 2;
                --remaining;
            } while (remaining);
        }
        newBuffer = allocated;
    }

    int usedCount = *usedField;
    if (usedCount != 0)
    {
        std::memcpy(newBuffer, *dataField, static_cast<std::size_t>(8 * usedCount));
    }

    const void* result = *dataField;
    bool wasNull = (*dataField == nullptr);
    *capacityField += extraCount;

    if (!wasNull)
    {
        Mem_Free(const_cast<void*>(result));
        *dataField = nullptr;
    }
    *dataField = newBuffer;
    return result;
}

HOOK(0x0045BBA0, ResizePairBuffer, "auto");
