#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Returns the byte at +0xBD. On first use (+0xBD == 0) it latches the byte at +0x6C
// into +0xBC and marks +0xBD as set, then returns the latched value.
static char __fastcall NetCogibleDDL_GetLazyLatchedByte(uint8_t* self, void* /*edx*/)
{
    char result = CSF_AT(char, self, 0xBD);
    if (!result)
    {
        result = CSF_AT(char, self, 0x6C);
        CSF_AT(char, self, 0xBC) = result;
        CSF_AT(char, self, 0xBD) = 1;
    }
    return result;
}

HOOK(0x00740130, NetCogibleDDL_GetLazyLatchedByte, "auto");
