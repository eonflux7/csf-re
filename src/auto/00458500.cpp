#include <cstdint>
#include "hook.h"

// Copies a five-dword record from source into destination when destination is non-null.
// Returns destination unchanged.
static uint32_t* __stdcall HudStatusIcons_CopyIconRecord(uint32_t* destination, const uint32_t* source)
{
    uint32_t* result = destination;
    if (destination)
    {
        destination[0] = source[0];
        destination[1] = source[1];
        destination[2] = source[2];
        destination[3] = source[3];
        destination[4] = source[4];
    }
    return result;
}

HOOK(0x458500, HudStatusIcons_CopyIconRecord, "auto");
