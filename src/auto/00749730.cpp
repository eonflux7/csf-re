#include <cstdint>
#include "hook.h"

// Returns true when the byte at the given pointer is 'L' (0x4C).
// A null pointer yields false.
static bool __cdecl MenuMpSessions_HasNetworkTypeTwo(uint8_t* typeByte) {
    bool result = false;
    if (typeByte) {
        return *typeByte == 0x4C;
    }
    return result;
}

HOOK(0x00749730, MenuMpSessions_HasNetworkTypeTwo, "auto");
