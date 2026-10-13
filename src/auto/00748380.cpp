#include <cstdint>
#include "globals.h"
#include "hook.h"

// Stores (a2 != 0) into the byte at +0x5F and returns the same value.
static bool __fastcall Character_SetNonZeroFlag(void* self, void* /*edx*/, char a2)
{
    bool isNonZero = (a2 != 0);
    CSF_AT(uint8_t, self, 0x5F) = isNonZero ? 1 : 0;
    return isNonZero;
}

HOOK(0x00748380, Character_SetNonZeroFlag, "auto");
