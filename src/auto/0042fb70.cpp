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

// Container layout (self):
//   +0x00  pointer to a block of 20-byte records
//   +0x04  record count (grows by extraCount)
//   +0x08  number of records copied from the old block
// Record layout (20 bytes): +0x00, +0x04, +0x08, +0x0C zeroed, +0x10 = 99.
static int __fastcall Hud_ExtendRecordArray(void* self, void* /*edx*/, int extraCount)
{
    const int newCount = CSF_AT(int, self, 0x4) + extraCount;

    void* allocatedBlock = Mem_Alloc(static_cast<std::size_t>(20 * newCount));
    void* newBuffer = nullptr;
    if (allocatedBlock != nullptr)
    {
        const int lastIndex = newCount - 1;
        if (lastIndex >= 0)
        {
            for (int i = 0; i <= lastIndex; ++i)
            {
                std::uint8_t* record = static_cast<std::uint8_t*>(allocatedBlock) + i * 20;
                CSF_AT(int, record, 0x00) = 0;
                CSF_AT(int, record, 0x04) = 0;
                CSF_AT(int, record, 0x08) = 0;
                CSF_AT(int, record, 0x0C) = 0;
                CSF_AT(int, record, 0x10) = 99;
            }
        }
        newBuffer = allocatedBlock;
    }

    const int copyCount = CSF_AT(int, self, 0x8);
    if (copyCount != 0)
    {
        std::memcpy(newBuffer, CSF_AT(void*, self, 0x0), static_cast<std::size_t>(20 * copyCount));
    }

    void* oldBuffer = CSF_AT(void*, self, 0x0);
    CSF_AT(int, self, 0x4) += extraCount;

    int result;
    if (oldBuffer == nullptr)
    {
        result = static_cast<int>(reinterpret_cast<std::uintptr_t>(newBuffer));
    }
    else
    {
        result = Mem_Free(oldBuffer);
        CSF_AT(void*, self, 0x0) = nullptr;
    }
    CSF_AT(void*, self, 0x0) = newBuffer;
    return result;
}

HOOK(0x0042FB70, Hud_ExtendRecordArray, "auto");
