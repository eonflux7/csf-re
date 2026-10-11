#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Returns non-zero when the state code at +0x18 is 1 or 2.
static int __fastcall C_0083e5e8_IsStateOneOrTwo(C_0083e5e8* self, void* /*edx*/)
{
    uint32_t stateCode = CSF_AT(uint32_t, self, 0x18);
    return stateCode == 2 || stateCode == 1;
}

HOOK(0x4E1D30, C_0083e5e8_IsStateOneOrTwo, "auto");
