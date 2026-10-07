#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Reads the byte flag stored at offset 0x118 of the object.
// Original: mov al, byte ptr [ecx + 0x118] ; ret
static char __fastcall C_6d7430_GetByte118(void* self, void* /*edx*/)
{
    return CSF_AT(char, self, 0x118);
}

HOOK(0x6D7430, C_6d7430_GetByte118, "auto");
