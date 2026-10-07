#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Sets the byte flag at offset 0x48 of the object to 1 and reports success.
static char __fastcall SetFlag48(uint8_t* self, void* /*edx*/)
{
    CSF_AT(uint8_t, self, 0x48) = 1;
    return 1;
}

HOOK(0x00753490, SetFlag48, "auto");
