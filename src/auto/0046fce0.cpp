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

// Element stride of the std::_String_base entries walked here.
static const std::size_t kStringBaseStride = 0x1C;

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0,
         (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));

// Copies the string elements in [begin, end) into the block ending at dest, walking backwards
// from end to begin. Each destination element receives the full source string.
// Returns the last destination element written (dest itself when the range is empty).
static void* __cdecl MissionScript_CopyStringRangeBackward(
    void* begin, void* end, void* dest, int /*unused*/)
{
    if (begin == end) {
        return dest;
    }

    std::uint8_t* sourceCursor = static_cast<std::uint8_t*>(end);
    std::uint8_t* destCursor = static_cast<std::uint8_t*>(dest);
    do {
        sourceCursor -= kStringBaseStride;
        destCursor -= kStringBaseStride;
        AssignStringSubstring(destCursor, nullptr, sourceCursor, 0, 0xFFFFFFFFu);
    } while (sourceCursor != static_cast<std::uint8_t*>(begin));

    return destCursor;
}

HOOK(0x0046FCE0, MissionScript_CopyStringRangeBackward, "auto");
