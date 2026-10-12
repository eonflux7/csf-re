#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// AssignStringSubstring(this=dst, source, position=0, count=-1) in the original.
CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0,
         (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));

// WaterEntry_PrepareFromDescriptor(this=dst+0x1C, a2=source+0x1C) in the original.
CSF_FUNC(void*, __fastcall, WaterEntry_PrepareFromDescriptor, 0x006FE280,
         (void* self, void* edx, void* a2));

// Walks the 168-byte water entry elements from rangeEnd back down to rangeBegin,
// copying each into the matching slot counting down from destEnd. Returns the
// last destination slot written, or destEnd when the range is empty.
static void* __cdecl CopyWaterEntryRangeReverse(void* rangeBegin, void* rangeEnd, void* destEnd, int /*unused*/)
{
    if (rangeBegin == rangeEnd)
        return destEnd;

    std::uint8_t* sourceCursor = static_cast<std::uint8_t*>(rangeEnd);
    std::uint8_t* destCursor = static_cast<std::uint8_t*>(destEnd);

    do
    {
        sourceCursor -= 168;
        destCursor -= 168;

        AssignStringSubstring(destCursor, nullptr, sourceCursor, 0u, 0xFFFFFFFFu);
        WaterEntry_PrepareFromDescriptor(destCursor + 28, nullptr, sourceCursor + 28);
    } while (sourceCursor != static_cast<std::uint8_t*>(rangeBegin));

    return destCursor;
}

HOOK(0x7758C0, CopyWaterEntryRangeReverse, "auto");
