#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// AssignStringSubstring(this=dst, source, position=0, count=-1) in the original.
CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0,
         (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));

// sub_772500(this=dst+0x1C, a5=source+0x1C) in the original.
CSF_FUNC(int, __fastcall, AnimationEntry_CopyTrailingBlock, 0x00772500,
         (void* self, void* edx, int a5));

// Walks the 44-byte animation entry elements from rangeEnd back down to rangeBegin,
// copying each into the matching slot counting down from destEnd. Returns the
// last destination slot written, or destEnd when the range is empty.
static void* __cdecl AnimationDefs_CopyEntryRangeReverse(void* rangeBegin, void* rangeEnd, void* destEnd, int /*unused*/)
{
    if (rangeBegin == rangeEnd)
        return destEnd;

    std::uint8_t* sourceCursor = static_cast<std::uint8_t*>(rangeEnd);
    std::uint8_t* destCursor = static_cast<std::uint8_t*>(destEnd);

    do
    {
        sourceCursor -= 44;
        destCursor -= 44;

        AssignStringSubstring(destCursor, nullptr, sourceCursor, 0u, 0xFFFFFFFFu);
        AnimationEntry_CopyTrailingBlock(destCursor + 28, nullptr, reinterpret_cast<int>(sourceCursor + 28));
    } while (sourceCursor != static_cast<std::uint8_t*>(rangeBegin));

    return destCursor;
}

HOOK(0x772710, AnimationDefs_CopyEntryRangeReverse, "auto");
