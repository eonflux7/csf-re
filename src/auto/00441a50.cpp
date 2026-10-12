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

// Converts a 32-bit guest address held in an int/uint32 to a host pointer.
static uint8_t* DynObj_AddrToPtr(uint32_t address) {
    return reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(address));
}

// Copies 16-byte entries backwards from the end of the source range (a2) down to a1,
// into the destination range ending at a3. Each entry is { data ptr, capacity, count, extra }.
// Reallocates a destination's data block when the source count exceeds the destination's
// capacity. Returns the destination entry that was written last, or a3 if the range is empty.
static int DynamicObj_CopyEntriesBackward(int a1, int a2, int a3, int a4) {
    (void)a4;

    if (a1 == a2) {
        return a3;
    }

    uint32_t srcEntry = static_cast<uint32_t>(a2);
    uint32_t dstEntry = static_cast<uint32_t>(a3);

    do {
        srcEntry -= 16;
        dstEntry -= 16;

        uint8_t* src = DynObj_AddrToPtr(srcEntry);
        uint8_t* dst = DynObj_AddrToPtr(dstEntry);

        uint32_t srcCount = CSF_AT(uint32_t, src, 0x8);
        uint32_t dstCapacity = CSF_AT(uint32_t, dst, 0x4);

        if (srcCount > dstCapacity) {
            uint32_t srcCapacity = CSF_AT(uint32_t, src, 0x4);
            void* newBlock = Mem_Alloc(4 * static_cast<std::size_t>(srcCapacity));
            void* oldBlock = reinterpret_cast<void*>(static_cast<uintptr_t>(CSF_AT(uint32_t, dst, 0x0)));
            CSF_AT(uint32_t, dst, 0x4) = srcCapacity;
            if (oldBlock != nullptr) {
                Mem_Free(oldBlock);
                CSF_AT(uint32_t, dst, 0x0) = 0;
            }
            CSF_AT(uint32_t, dst, 0x0) = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(newBlock));
        }

        uint32_t copyCount = CSF_AT(uint32_t, src, 0x8);
        if (copyCount != 0) {
            uint8_t* srcData = DynObj_AddrToPtr(CSF_AT(uint32_t, src, 0x0));
            uint8_t* dstData = DynObj_AddrToPtr(CSF_AT(uint32_t, dst, 0x0));
            std::memcpy(dstData, srcData, 4 * static_cast<std::size_t>(copyCount));
        }

        CSF_AT(uint32_t, dst, 0x8) = CSF_AT(uint32_t, src, 0x8);
        CSF_AT(uint32_t, dst, 0xC) = CSF_AT(uint32_t, src, 0xC);
    } while (srcEntry != static_cast<uint32_t>(a1));

    return static_cast<int>(dstEntry);
}

HOOK(0x441A50, DynamicObj_CopyEntriesBackward, "auto");
