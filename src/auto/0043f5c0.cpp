#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Assigns the substring [position, position + count) of source into the string at self.
// count == -1 (0xFFFFFFFF) selects the rest of the source string.
CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0,
         (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));

// Walks a block of 56-byte records backwards from endRecord down to startRecord (exclusive
// of startRecord itself, which is the stop value), clearing the two embedded strings of each
// record (offset 0 and offset 28) by assigning empty substrings. Returns the lowest record
// pointer reached (the walked-down base), or destBase unchanged when the range is empty.
static int Campaign_ClearStringPairRange(int startRecord, int endRecord, int destBase)
{
    std::uint32_t srcCursor = static_cast<std::uint32_t>(endRecord);
    if (static_cast<int>(srcCursor) == startRecord)
        return destBase;

    std::uint32_t destCursor = static_cast<std::uint32_t>(destBase);
    do
    {
        srcCursor -= 56;
        destCursor -= 56;

        AssignStringSubstring(reinterpret_cast<void*>(destCursor), nullptr,
                              reinterpret_cast<const void*>(srcCursor), 0, 0xFFFFFFFFu);
        AssignStringSubstring(reinterpret_cast<void*>(destCursor + 28), nullptr,
                              reinterpret_cast<const void*>(srcCursor + 28), 0, 0xFFFFFFFFu);
    }
    while (static_cast<int>(srcCursor) != startRecord);

    return static_cast<int>(destCursor);
}

HOOK(0x43F5C0, Campaign_ClearStringPairRange, "auto");
