#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// 0x00480C10 - CCharWeaponFsm step, runs before the action dispatch.
CSF_FUNC(int, __fastcall, CCharWeaponFsm_StepWeaponState, 0x00480C10,
         (void* self, void* /*edx*/, int a2));

// 0x00488210 - action-state dispatch; its result is the function's result.
CSF_FUNC(int, __fastcall, CCharActionFsm_DispatchActionState, 0x00488210,
         (void* self, void* /*edx*/, int a2));

// 0x00488AD0 - CCharActionFsm update: runs the weapon-FSM step, then the
// action-state dispatch, forwarding the dispatch result unchanged.
static int __fastcall CCharActionFsm_StepWeaponThenDispatchAction(CCharActionFsm* self, void* /*edx*/, int a2)
{
    CCharWeaponFsm_StepWeaponState(self, nullptr, a2);
    return CCharActionFsm_DispatchActionState(self, nullptr, a2);
}

HOOK(0x488AD0, CCharActionFsm_StepWeaponThenDispatchAction, "auto");
