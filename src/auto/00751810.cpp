#include <cstdint>
#include "globals.h"
#include "hook.h"

// Sets the byte flag at offset 0x20C of the object and returns 1.
static char __fastcall SetFlag20C(void* self, void* /*edx*/)
{
    CSF_AT(uint8_t, self, 0x20C) = 1;
    return 1;
}

HOOK(0x751810, SetFlag20C, "auto");
