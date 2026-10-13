#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callee at 0x0048CAD0: __fastcall-style body taking (self in ecx, char, float), ret 8.
CSF_FUNC(bool, __fastcall, AiAttackTasks_ComputeWeaponAction, 0x0048CAD0, (void* self, void* edx, char a2, float a3));

// Forwards the caller's char argument with a fixed angular threshold of
// 0.17453292f radians (about 10 degrees, pi/18) to the shared action check.
// The original leaves ecx (this) untouched, so self is passed through as-is.
static int __fastcall AiAttackTasks_IsWeaponActionAvailable(S_0048cad0* self, void* /*edx*/, char a2)
{
    return AiAttackTasks_ComputeWeaponAction(self, nullptr, a2, 0.17453292f) ? 1 : 0;
}

HOOK(0x0048D030, AiAttackTasks_IsWeaponActionAvailable, "auto");
