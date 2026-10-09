#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));

// Copies the string-bearing elements in [rangeBegin, rangeEnd) into the block ending at destEnd,
// walking backwards in 32-byte steps. Each element's string is assigned in full, and its
// trailing 32-bit field at +0x1C is copied as well. Returns the last destination element written,
// or destEnd when the range is empty.
static void* __cdecl MissionScript_CopyStringElementsReverse(
    void* rangeBegin, void* rangeEnd, void* destEnd, int /*unused*/)
{
    if (rangeBegin == rangeEnd)
        return destEnd;

    std::uint8_t* srcCursor = static_cast<std::uint8_t*>(rangeEnd);
    std::uint8_t* dstCursor = static_cast<std::uint8_t*>(destEnd);
    do
    {
        srcCursor -= 32;
        dstCursor -= 32;
        AssignStringSubstring(dstCursor, nullptr, srcCursor, 0, 0xFFFFFFFFu);
        CSF_AT(std::uint32_t, dstCursor, 0x1C) = CSF_AT(std::uint32_t, srcCursor, 0x1C);
    } while (srcCursor != static_cast<std::uint8_t*>(rangeBegin));

    return dstCursor;
}

HOOK(0x0046C7E0, MissionScript_CopyStringElementsReverse, "auto");
