#include <cstdint>
#include "hook.h"

static bool __cdecl IsNotAsciiAlphanumeric(std::uint16_t character)
{
    return (character < 0x30u || character > 0x39u)
        && (character < 0x61u || character > 0x7Au)
        && (character < 0x41u || character > 0x5Au);
}

HOOK(0x007210D0, IsNotAsciiAlphanumeric, "auto");
