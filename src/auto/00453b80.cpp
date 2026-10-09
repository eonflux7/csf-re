#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __fastcall, sub_4525E0, 0x004525E0, (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));

// Walks two arrays of 48-byte records backwards in lockstep, from sourceEnd/destEnd
// down to sourceStop. Each record's sub-object at +4 is handled by sub_4525E0 and its
// string at +20 is replaced with a full copy of the source string.
// Returns the destination cursor after the last record was processed (or destEnd if
// the range is empty).
static int __cdecl CopyRecordRangeBackward(int sourceStop, int sourceEnd, int destEnd, int unusedArg4)
{
    (void)unusedArg4;

    if (sourceStop == sourceEnd)
        return destEnd;

    std::uintptr_t srcCursor = static_cast<std::uintptr_t>(static_cast<std::uint32_t>(sourceEnd));
    std::uintptr_t dstCursor = static_cast<std::uintptr_t>(static_cast<std::uint32_t>(destEnd));
    const std::uintptr_t stop = static_cast<std::uintptr_t>(static_cast<std::uint32_t>(sourceStop));

    do
    {
        srcCursor -= 48;
        dstCursor -= 48;

        sub_4525E0(reinterpret_cast<void*>(dstCursor + 4), nullptr, static_cast<int>(srcCursor + 4));
        AssignStringSubstring(reinterpret_cast<void*>(dstCursor + 20), nullptr,
                              reinterpret_cast<const void*>(srcCursor + 20), 0u, 0xFFFFFFFFu);
    } while (srcCursor != stop);

    return static_cast<int>(dstCursor);
}

HOOK(0x00453B80, CopyRecordRangeBackward, "auto");
