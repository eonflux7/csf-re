#include <cstdint>
#include "globals.h"
#include "hook.h"

// Stores -1 into the dword at offset 0x2B0 of the object (ecx = this, no stack args).
static void __fastcall SetField2B0ToMinusOne(void* self, void* /*edx*/)
{
    CSF_AT(int32_t, self, 0x2B0) = -1;
}

HOOK(0x006A7150, SetField2B0ToMinusOne, "auto");
