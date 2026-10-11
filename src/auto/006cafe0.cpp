#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Reallocates a table of 8-byte (index, -1) pairs to hold (count + extraCount) entries,
// copies the existing entries over, frees the old table and updates the count.
static const void* __fastcall GfxScene_ReallocPairTable(void* self, void* /*edx*/, int extraCount)
{
    int newCount = CSF_AT(int, self, 4) + extraCount;

    void* newBlock = Mem_Alloc(static_cast<std::size_t>(newCount) * 8);
    void* fillBlock = nullptr;
    if (newBlock != nullptr)
    {
        uint32_t* pair = static_cast<uint32_t*>(newBlock);
        if (newCount - 1 >= 0)
        {
            int remaining = newCount;
            do
            {
                pair[0] = 0;
                pair[1] = 0xFFFFFFFFu;
                pair += 2;
                --remaining;
            } while (remaining != 0);
        }
        fillBlock = newBlock;
    }

    uint32_t oldCount = CSF_AT(uint32_t, self, 8);
    if (oldCount != 0)
    {
        std::memcpy(fillBlock, CSF_AT(const void*, self, 0), static_cast<std::size_t>(oldCount) * 8);
    }

    const void* result = CSF_AT(const void*, self, 0);
    bool hadOldBlock = (result != nullptr);
    CSF_AT(int, self, 4) += extraCount;
    if (hadOldBlock)
    {
        result = reinterpret_cast<const void*>(Mem_Free(const_cast<void*>(result)));
        CSF_AT(void*, self, 0) = nullptr;
    }
    CSF_AT(void*, self, 0) = fillBlock;
    return result;
}

HOOK(0x006CAFE0, GfxScene_ReallocPairTable, "auto");
