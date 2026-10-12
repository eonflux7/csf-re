#include <cstdint>
#include "hook.h"

static std::uint32_t* __cdecl GfxScreenEffects_ShiftTimedEntriesLeft(
    std::uint32_t* first,
    std::uint32_t* last,
    std::uint32_t* destination,
    int /*unused*/)
{
    while (first != last)
    {
        destination[0] = first[0];
        destination[1] = first[1];
        destination[2] = first[2];
        destination[3] = first[3];
        const std::uint32_t finalWord = first[4];
        std::uint32_t* entry = destination;
        first += 5;
        destination += 5;
        entry[4] = finalWord;
    }
    return destination;
}

HOOK(0x006F9FE0, GfxScreenEffects_ShiftTimedEntriesLeft, "auto");
