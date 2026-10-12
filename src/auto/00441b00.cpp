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

// Copies the element arrays of the source record into every 16-byte entry in [beginAddr, endAddr).
// Entry layout: +0 data pointer, +4 allocated element count, +8 used element count, +12 extra value.
// Source record (a3) has the same layout: +0 data pointer, +4 element count to allocate, +8 element count to copy.
static int __cdecl DynamicObj_CopyDataRange(int beginAddr, int endAddr, int sourceAddr)
{
    const uint32_t endValue = static_cast<uint32_t>(endAddr);
    int result = endAddr;

    for (uintptr_t entryAddr = static_cast<uint32_t>(beginAddr);
         entryAddr != endValue;
         entryAddr += 16)
    {
        uint8_t* entry = reinterpret_cast<uint8_t*>(entryAddr);
        uint8_t* source = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(static_cast<uint32_t>(sourceAddr)));

        if (static_cast<uint32_t>(CSF_AT(int, source, 8)) > static_cast<uint32_t>(CSF_AT(int, entry, 4)))
        {
            void* newBlock = Mem_Alloc(static_cast<std::size_t>(CSF_AT(int, source, 4)) * 4);
            void* oldBlock = CSF_AT(void*, entry, 0);
            CSF_AT(int, entry, 4) = CSF_AT(int, source, 4);
            if (oldBlock != nullptr)
            {
                Mem_Free(oldBlock);
                CSF_AT(void*, entry, 0) = nullptr;
            }
            CSF_AT(void*, entry, 0) = newBlock;
        }

        int copyCount = CSF_AT(int, source, 8);
        if (copyCount != 0)
        {
            std::memcpy(CSF_AT(void*, entry, 0),
                        CSF_AT(const void*, source, 0),
                        static_cast<std::size_t>(copyCount) * 4);
        }

        CSF_AT(int, entry, 8) = CSF_AT(int, source, 8);
        result = endAddr;
        CSF_AT(int, entry, 12) = CSF_AT(int, source, 12);
    }

    return result;
}

HOOK(0x00441B00, DynamicObj_CopyDataRange, "auto");
