#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Substring assignment on a text string object (source, position, count).
CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));

// Copies the 0x20-byte sub-block at +0x20 of a text entry.
CSF_FUNC(int, __fastcall, TextEntry_CopyHeaderBlock, 0x004EFF30, (void* self, void* edx, void* a5));

// Prepares and applies the 0x30-byte sub-object at +0x30 of a text entry.
CSF_FUNC(unsigned int*, __fastcall, S_006b0320_PrepareAndApply, 0x006B0B70, (void* self, void* , int a2));

// Copies a contiguous range of 104-byte text entries from [first, last) to dest.
// Returns the end pointer of the written destination range, or dest if the range is empty.
static int __cdecl CopyTextEntryRange(S_004a31e0* first, S_004a31e0* last, int dest)
{
    if (first == last)
        return dest;

    uint8_t* srcCur = reinterpret_cast<uint8_t*>(first);
    uint8_t* endPtr = reinterpret_cast<uint8_t*>(last);
    uint8_t* dstCur = reinterpret_cast<uint8_t*>(dest);

    do
    {
        CSF_AT(uint32_t, dstCur, 0x00) = CSF_AT(uint32_t, srcCur, 0x00);

        AssignStringSubstring(dstCur + 0x04, nullptr, srcCur + 0x04, 0u, 0xFFFFFFFFu);

        TextEntry_CopyHeaderBlock(dstCur + 0x20, nullptr, srcCur + 0x20);

        S_006b0320_PrepareAndApply(dstCur + 0x30, nullptr, static_cast<int>(reinterpret_cast<std::uintptr_t>(srcCur + 0x30)));

        CSF_AT(uint32_t, dstCur, 0x50) = CSF_AT(uint32_t, srcCur, 0x50);
        CSF_AT(uint32_t, dstCur, 0x54) = CSF_AT(uint32_t, srcCur, 0x54);
        CSF_AT(uint32_t, dstCur, 0x58) = CSF_AT(uint32_t, srcCur, 0x58);
        CSF_AT(uint32_t, dstCur, 0x5C) = CSF_AT(uint32_t, srcCur, 0x5C);
        CSF_AT(uint32_t, dstCur, 0x60) = CSF_AT(uint32_t, srcCur, 0x60);
        CSF_AT(uint32_t, dstCur, 0x64) = CSF_AT(uint32_t, srcCur, 0x64);

        srcCur += 104;
        dstCur += 104;
    }
    while (srcCur != endPtr);

    return static_cast<int>(reinterpret_cast<std::uintptr_t>(dstCur));
}

HOOK(0x004F02C0, CopyTextEntryRange, "auto");
