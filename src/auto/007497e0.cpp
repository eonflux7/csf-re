#include <cstdint>
#include "hook.h"

// Returns true when the byte at offset 1 of the given record is 'P' (0x50).
// A null pointer yields false.
static bool __cdecl MenuMpSessions_HasSecondaryFilterFlag(uint8_t* record) {
    bool result = false;
    if (record) {
        return record[1] == 0x50;
    }
    return result;
}

HOOK(0x007497E0, MenuMpSessions_HasSecondaryFilterFlag, "auto");
