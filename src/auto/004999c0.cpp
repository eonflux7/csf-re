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

CSF_FUNC(int, __fastcall, sub_4993E0, 0x004993E0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, IsFieldAt10Equal36, 0x00483360, (void* self, void*));
CSF_FUNC(int, __fastcall, CCharActionFsm_UpdateWeaponActionState, 0x004849A0, (CCharActionFsm* self, void*));
CSF_FUNC(int, __fastcall, sub_492AE0, 0x00492AE0, (void* self, void* edx, int a2, int a3));

// `self` is a secondary base embedded at +4 of the owning object.
static int __fastcall CCharActionFsm_ResolveActionState(void* self, void* /*edx*/)
{
    CCharActionFsm* actionFsm = CSF_AT(CCharActionFsm*, self, 0x40);
    uint8_t* owner = static_cast<uint8_t*>(self) - 4;

    // A pending action is set: hand off to the owner's pending-action handler.
    if (CSF_AT(int, self, 0x1B8) >= 0 && CSF_AT(int, self, 0x1BC) != -1)
    {
        return sub_4993E0(owner, nullptr);
    }

    int result = 0;
    bool actionReady = false;
    if (CSF_AT(int, self, 0x294) != 0)
    {
        // Virtual readiness check on the action FSM: vtable offset 0x90 = slot 36.
        result = ((int(__fastcall*)(void*, void*))CSF_AT(void**, actionFsm, 0)[36])(actionFsm, nullptr);
        actionReady = static_cast<uint8_t>(result) != 0;
    }

    if (actionReady)
    {
        // Inlined tail call (0x4952C0) on the owning object.
        if (CSF_AT(int, owner, 0x298) == 7)
        {
            result = sub_492AE0(owner, nullptr,
                                reinterpret_cast<int>(owner + 0x29C),
                                CSF_AT(int, owner, 0x2A8));
            CSF_AT(uint32_t, owner, 0x2E4) |= 0x2000;
        }
        else
        {
            CSF_AT(uint32_t, owner, 0x2E4) |= 0x20;
        }
        return result;
    }

    result = IsFieldAt10Equal36(actionFsm, nullptr);
    if (static_cast<uint8_t>(result) != 0)
        return CCharActionFsm_UpdateWeaponActionState(actionFsm, nullptr);
    return result;
}

HOOK(0x004999C0, CCharActionFsm_ResolveActionState, "auto");
