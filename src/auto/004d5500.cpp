#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callee at 0x004D05E0: __fastcall (self in ecx, edx passed through), ret 8.
CSF_FUNC(bool, __fastcall, C_00835228_IsTargetPositionClear, 0x004D05E0, (C_00835228* self, void* , int targetHandle, int positionIndex));

// Checks whether the target handle (stack argument) is clear at fixed
// position index 13. ecx (this) and edx are forwarded unchanged to the callee.
static bool __fastcall C_0083c280_IsTargetPositionClear(C_00835228* self, void* edx, int targetHandle)
{
    return C_00835228_IsTargetPositionClear(self, edx, targetHandle, 13);
}

HOOK(0x004D5500, C_0083c280_IsTargetPositionClear, "auto");
