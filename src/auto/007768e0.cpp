#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// AssignStringSubstring(this=dst, source, position=0, count=-1) in the original.
CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0,
         (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));

// AssignTableWordVector(this=dst+0x1C, source+0x1C) in the original.
CSF_FUNC(int, __fastcall, AssignTableWordVector, 0x0051A020,
         (S_0046c410* self, void* , void* source));

// Walks the 44-byte string-base elements from rangeEnd back down to rangeBegin,
// copying each into the matching slot counting down from destEnd. Returns the
// last destination slot written, or destEnd when the range is empty.
static void* __cdecl CopyStringElementsReverse(void* rangeBegin, void* rangeEnd, void* destEnd, int /*unused*/)
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
        AssignTableWordVector(reinterpret_cast<S_0046c410*>(destCursor + 28), nullptr, sourceCursor + 28);
    } while (sourceCursor != static_cast<std::uint8_t*>(rangeBegin));

    return destCursor;
}

HOOK(0x7768E0, CopyStringElementsReverse, "auto");
