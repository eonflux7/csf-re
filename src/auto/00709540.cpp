#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static uint8_t* __fastcall C_00862b58_PopulateOpaqueColorTable(
    C_00862b58* self, void* /*edx*/)
{
    uint8_t* destination = reinterpret_cast<uint8_t*>(self) + 0x68;
    uint8_t* source = reinterpret_cast<uint8_t*>(CSF_ADDR_DATA(0x008A81AD));
    uint8_t* end = reinterpret_cast<uint8_t*>(CSF_ADDR_DATA(0x008A85AD));

    do
    {
        CSF_AT(uint32_t, destination - 4, 0) =
            source[-3] |
            ((source[-4] | ((source[-5] | 0xFFFFFF00u) << 8)) << 8);

        CSF_AT(uint32_t, destination, 0) =
            source[1] |
            ((source[0] | ((source[-1] | 0xFFFFFF00u) << 8)) << 8);

        CSF_AT(uint32_t, destination, 4) =
            source[5] |
            ((source[4] | ((source[3] | 0xFFFFFF00u) << 8)) << 8);

        CSF_AT(uint32_t, destination, 8) =
            source[9] |
            ((source[8] | ((source[7] | 0xFFFFFF00u) << 8)) << 8);

        source += 16;
        destination += 16;
    }
    while (reinterpret_cast<intptr_t>(source) < reinterpret_cast<intptr_t>(end));

    return source;
}

HOOK(0x00709540, C_00862b58_PopulateOpaqueColorTable, "auto");
