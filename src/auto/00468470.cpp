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

// Copies a range of 16-byte records [srcBegin, srcEnd) into a destination record cursor.
// Each record: +0x00 data block pointer (uint32 elements), +0x04 capacity, +0x08 count, +0x0C extra value.
// Grows the destination block when the source count exceeds its capacity.
// Returns the destination cursor advanced past the last written record.
static int __cdecl MissionScript_CopyRecordRange(int srcBegin, int srcEnd, int dstCursor)
{
    if (srcBegin == srcEnd) {
        return dstCursor;
    }

    do {
        uint8_t* src = reinterpret_cast<uint8_t*>(static_cast<std::uintptr_t>(srcBegin));
        uint8_t* dst = reinterpret_cast<uint8_t*>(static_cast<std::uintptr_t>(dstCursor));

        if (CSF_AT(uint32_t, src, 0x08) > CSF_AT(uint32_t, dst, 0x04)) {
            void* newBlock = Mem_Alloc(4 * static_cast<std::size_t>(CSF_AT(uint32_t, src, 0x04)));
            void* oldBlock = CSF_AT(void*, dst, 0x00);
            CSF_AT(uint32_t, dst, 0x04) = CSF_AT(uint32_t, src, 0x04);
            if (oldBlock != nullptr) {
                Mem_Free(oldBlock);
                CSF_AT(void*, dst, 0x00) = nullptr;
            }
            CSF_AT(void*, dst, 0x00) = newBlock;
        }

        uint32_t count = CSF_AT(uint32_t, src, 0x08);
        if (count != 0) {
            std::memcpy(CSF_AT(void*, dst, 0x00),
                        CSF_AT(void*, src, 0x00),
                        4 * static_cast<std::size_t>(count));
        }

        CSF_AT(uint32_t, dst, 0x08) = CSF_AT(uint32_t, src, 0x08);
        CSF_AT(uint32_t, dst, 0x0C) = CSF_AT(uint32_t, src, 0x0C);

        dstCursor += 16;
        srcBegin += 16;
    } while (srcBegin != srcEnd);

    return dstCursor;
}

HOOK(0x468470, MissionScript_CopyRecordRange, "auto");
