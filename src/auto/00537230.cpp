#include <cstdint>
#include "globals.h"
#include "hook.h"

// Returns the byte flag/value at offset 0x185 of the object (offset is
// somewhere inside a pad_/unknown region of its class).
static char __fastcall C_Unknown_GetByte185(void* self, void* /*edx*/)
{
    return static_cast<char>(CSF_AT(uint8_t, self, 0x185));
}

HOOK(0x00537230, C_Unknown_GetByte185, "auto");
