#include <cstdint>
#include "hook.h"

// Returns true when the byte at offset +3 of the given record is 'F' (0x46).
// A null pointer yields false.
static bool __cdecl MenuMpSessions_HasFlagFAtOffset3(uint8_t* recordPtr) {
    bool result = false;
    if (recordPtr) {
        return recordPtr[3] == 0x46;
    }
    return result;
}

HOOK(0x00749750, MenuMpSessions_HasFlagFAtOffset3, "auto");
