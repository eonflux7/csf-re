#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Callee bindings (by original address)
CSF_FUNC(int, __fastcall, CCharActionFsm_UpdateWeaponActionState, 0x004849A0, (CCharActionFsm* self, void*));
CSF_FUNC(int, __fastcall, ApplyMenuContextState, 0x00499F00, (void* self, void* edx, char a2));

// Refreshes the menu context: updates the weapon action state of the embedded
// action FSM, runs the virtual refresh hook (slot 0x54), then applies state 0.
static int __fastcall RefreshMenuContext(C_00833b88* self, void* /*edx*/)
{
    CCharActionFsm_UpdateWeaponActionState(reinterpret_cast<CCharActionFsm*>(self->field_0044), nullptr);
    self->vSlot54();
    return ApplyMenuContextState(self, nullptr, 0);
}

HOOK(0x0049A7D0, RefreshMenuContext, "auto");
