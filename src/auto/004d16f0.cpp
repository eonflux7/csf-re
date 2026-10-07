#include <cstdint>

#include "globals.h"
#include "hook.h"

// Sets the object byte at +0x00 to 0, both dword slots at +0x14 and +0x18 to -1,
// and returns -1.
static int __fastcall ResetState(void* self, void* /*edx*/)
{
    CSF_AT(uint8_t, self, 0x00) = 0;
    CSF_AT(int32_t, self, 0x14) = -1;
    CSF_AT(int32_t, self, 0x18) = -1;
    return -1;
}

HOOK(0x4D16F0, ResetState, "auto");
