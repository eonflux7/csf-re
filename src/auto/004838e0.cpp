#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callee at 0x00494E90: receives the weapon FSM's field_0008 as its `this`-like
// first argument and 0 as its second.
CSF_FUNC(int, __fastcall, sub_494E90, 0x00494E90, (void* self, void* edx, int a4));

// Hook for 0x004838E0 (__thiscall, ret 0).
// 1) Virtual slot 0x11C with argument 13 (first, before the field read).
// 2) Re-read field_0008 after that call and call sub_494E90(field_0008, 0).
// Returns sub_494E90's result.
static int __fastcall CCharWeaponFsm_NotifyStateThirteenThenQuery(CCharWeaponFsm* self, void* /*edx*/)
{
    self->vUpdateWeaponSelection(13);
    return sub_494E90(reinterpret_cast<void*>(self->field_0008), nullptr, 0);
}

HOOK(0x004838E0, CCharWeaponFsm_NotifyStateThirteenThenQuery, "auto");
