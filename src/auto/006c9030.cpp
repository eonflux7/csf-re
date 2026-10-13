#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));

// Copies a range of 36-byte records backward from rangeEnd down to rangeStart
// into the block ending at destinationEnd. Each record gets its string reassigned
// from the source (full substring, count -1) plus two trailing fields copied.
// Returns the destination cursor after the copy (or destinationEnd if the range is empty).
static int __cdecl PopulateReversedRecordRange(int rangeStart, int rangeEnd, int destinationEnd)
{
    const std::uint8_t* rangeBegin = reinterpret_cast<const std::uint8_t*>(static_cast<std::uintptr_t>(rangeStart));
    std::uint8_t* sourceCursor = reinterpret_cast<std::uint8_t*>(static_cast<std::uintptr_t>(rangeEnd));

    if (rangeStart == rangeEnd)
        return destinationEnd;

    std::uint8_t* destCursor = reinterpret_cast<std::uint8_t*>(static_cast<std::uintptr_t>(destinationEnd));
    do
    {
        sourceCursor -= 36;
        destCursor -= 36;
        AssignStringSubstring(destCursor, nullptr, sourceCursor, 0u, 0xFFFFFFFFu);
        CSF_AT(std::uint32_t, destCursor, 0x1C) = CSF_AT(std::uint32_t, sourceCursor, 0x1C);
        CSF_AT(std::uint16_t, destCursor, 0x20) = CSF_AT(std::uint16_t, sourceCursor, 0x20);
    } while (sourceCursor != rangeBegin);

    return static_cast<int>(reinterpret_cast<std::uintptr_t>(destCursor));
}

HOOK(0x006C9030, PopulateReversedRecordRange, "auto");
