#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Sets or clears bit 0x04 of the byte at offset +2 of the object at a1.
// Returns the updated byte (al). __cdecl, pops nothing.
static char __cdecl GfxEnvironment_SetSectorObjectFlag4(int a1, char a2)
{
    uint8_t* object = reinterpret_cast<uint8_t*>(a1);
    char result;
    if (a2)
        result = CSF_AT(char, object, 2) | 4;
    else
        result = CSF_AT(char, object, 2) & 0xFB;
    CSF_AT(char, object, 2) = result;
    return result;
}

HOOK(0x6C0160, GfxEnvironment_SetSectorObjectFlag4, "auto");
