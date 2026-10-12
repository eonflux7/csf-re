#include <cstdint>
#include <cstddef>
#include "hook.h"

// Copies one byte from *source to *destination when destination is non-null.
// Returns destination unchanged (null stays null).
static uint8_t* __stdcall CopyByteToDestination(uint8_t* destination, uint8_t* source) {
    uint8_t* result = destination;
    if (destination) {
        *destination = *source;
    }
    return result;
}

HOOK(0x0051ADB0, CopyByteToDestination, "auto");
