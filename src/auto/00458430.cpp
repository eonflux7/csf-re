#include <cstdint>
#include <cstddef>
#include <cstring>
#include "hook.h"

// Shifts a range of 40-byte HUD status icon entries toward higher addresses
// by one slot, walking backward from rangeEnd down to rangeBegin. Each entry
// copies bytes 0..27 and the dwords at 32 and 36; bytes 28..31 are left alone.
// Returns the final destination cursor (or the original dest if the range is empty).
static char* __cdecl HudStatusIcons_MoveEntriesDown(char* rangeBegin, char* rangeEnd, char* dest, int /*unusedArg*/)
{
    char* srcCursor = rangeEnd;
    char* result = dest;
    if (rangeBegin != rangeEnd)
    {
        char* destTail = dest + 36;
        char* srcTail = rangeEnd + 36;
        do
        {
            srcCursor -= 40;
            srcTail -= 40;
            dest -= 40;
            std::memcpy(dest, srcCursor, 28);
            destTail -= 40;
            *reinterpret_cast<uint32_t*>(destTail - 4) = *reinterpret_cast<uint32_t*>(srcTail - 4);
            *reinterpret_cast<uint32_t*>(destTail) = *reinterpret_cast<uint32_t*>(srcTail);
        } while (srcCursor != rangeBegin);
        return dest;
    }
    return result;
}

HOOK(0x458430, HudStatusIcons_MoveEntriesDown, "auto");
