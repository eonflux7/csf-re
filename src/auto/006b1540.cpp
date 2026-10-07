#include <cstdint>
#include "globals.h"
#include "core/memory.h"
#include "hook.h"

// Stores the given byte at offset 0x50 of the object and returns it unchanged.
static char __fastcall SetByteAt0x50(void* self, void* /*edx*/, char value)
{
    CSF_AT(char, self, 0x50) = value;
    return value;
}

HOOK(0x6B1540, SetByteAt0x50, "auto");
