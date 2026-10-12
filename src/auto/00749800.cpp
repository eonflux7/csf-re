#include <cstdint>
#include "hook.h"

// Returns true when the object pointer is non-null and the byte at offset +4
// equals 'P' (0x50). A null pointer yields false.
static bool __cdecl MenuMpSessions_PassesFinalSessionCheck(int objectPtr) {
    bool result = false;
    if (objectPtr) {
        return *reinterpret_cast<uint8_t*>(objectPtr + 4) == 0x50;
    }
    return result;
}

HOOK(0x00749800, MenuMpSessions_PassesFinalSessionCheck, "auto");
