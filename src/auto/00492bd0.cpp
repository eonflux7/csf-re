#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Returns nonzero when any bit of the action-block mask (0x38) is set in the
// byte at offset 0x1F0 of the FSM object.
static int __fastcall C_00833908_IsWeaponActionBlocked(C_00833908* self, void* /*edx*/)
{
    return (CSF_AT(uint8_t, self, 0x1F0) & 0x38) != 0;
}

HOOK(0x00492BD0, C_00833908_IsWeaponActionBlocked, "auto");
