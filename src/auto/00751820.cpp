#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Clears the byte flag at offset 0x20C of the object.
static void __fastcall ClearByteFlag20C(void* self, void* /*edx*/)
{
    CSF_AT(uint8_t, self, 0x20C) = 0;
}

HOOK(0x751820, ClearByteFlag20C, "auto");
