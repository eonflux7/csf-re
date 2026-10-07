#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Table of 0x85 (133) 16-bit ids scanned linearly by the target function.
CSF_GLOBAL(uint16_t, g_IdTableStart, 0x008A8790);

// Returns TRUE when the given 16-bit id is NOT present in the 0x85-entry id table at 0x8A8790.
static int __cdecl IsIdNotInTable(int16_t id)
{
    unsigned int index = 0;
    while (index < 0x85)
    {
        if ((&g_IdTableStart)[index] == static_cast<uint16_t>(id))
            break;
        ++index;
    }
    return index >= 0x85;
}

HOOK(0x007210A0, IsIdNotInTable, "auto");
