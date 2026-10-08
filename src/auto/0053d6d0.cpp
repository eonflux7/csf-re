#include <cstdint>
#include <cstddef>
#include "hook.h"
#include "core/memory.h"
#include "globals.h"

// Game heap allocator used by the original (called via cdecl, 1 stack arg).
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (size_t byteCount));
// Game heap free, paired with Mem_Alloc.
CSF_FUNC(void, __cdecl, Mem_Free, 0x00691110, (void* block));

// Copies the element array and counters of `source` into `self`, reallocating
// the element buffer when `source` holds more elements than `self` can store.
// Element size is 8 bytes (two dwords). Layout: [0]=data, [4]=capacity,
// [8]=count, [12]=extra dword.
static void* __fastcall MenuMissionInfo_AssignFromArray(void* self, void* /*edx*/, void* source)
{
    if (CSF_AT(uint32_t, source, 0x08) > CSF_AT(uint32_t, self, 0x04))
    {
        void* newBuffer = Mem_Alloc(static_cast<size_t>(8u * CSF_AT(uint32_t, source, 0x04)));
        void* oldBuffer = CSF_AT(void*, self, 0x00);
        CSF_AT(uint32_t, self, 0x04) = CSF_AT(uint32_t, source, 0x04);
        if (oldBuffer != nullptr)
        {
            Mem_Free(oldBuffer);
            CSF_AT(void*, self, 0x00) = nullptr;
        }
        CSF_AT(void*, self, 0x00) = newBuffer;
    }

    uint32_t elementCount = CSF_AT(uint32_t, source, 0x08);
    if (elementCount != 0)
    {
        const uint32_t* sourceData = static_cast<const uint32_t*>(CSF_AT(void*, source, 0x00));
        uint32_t* destData = static_cast<uint32_t*>(CSF_AT(void*, self, 0x00));
        uint32_t dwordCount = elementCount * 2u;
        for (uint32_t i = 0; i < dwordCount; ++i)
            destData[i] = sourceData[i];
    }

    CSF_AT(uint32_t, self, 0x08) = CSF_AT(uint32_t, source, 0x08);
    CSF_AT(uint32_t, self, 0x0C) = CSF_AT(uint32_t, source, 0x0C);
    return self;
}

HOOK(0x0053D6D0, MenuMissionInfo_AssignFromArray, "auto");
