#include <cstdint>
#include "globals.h"
#include "hook.h"

// Original string-assign helper (__thiscall, 3 stack args -> ret 12).
CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0,
         (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));

// Copies each 28-byte string record from [first, last) into consecutive records starting at dest,
// assigning the full source string (position 0, count -1) to each. Returns the one-past-last dest record,
// or dest itself when the source range is empty.
static std::uint8_t* __cdecl CopyAssignStringRange(std::uint8_t* first, std::uint8_t* last,
                                                   std::uint8_t* dest, int /*unused*/) {
    if (first == last) {
        return dest;
    }

    std::uint8_t* sourceCursor = first;
    std::uint8_t* destCursor = dest;
    do {
        AssignStringSubstring(destCursor, nullptr, sourceCursor, 0, 0xFFFFFFFFu);
        sourceCursor += 0x1C;
        destCursor += 0x1C;
    } while (sourceCursor != last);

    return destCursor;
}

HOOK(0x00519190, CopyAssignStringRange, "auto");
