#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_00833908_ForwardRequest_48F2A0, 0x0048F2A0, (C_00833908* self, void* , int request, int arg2, int arg3, int arg4));
CSF_FUNC(int, __fastcall, CCharActionFsm_ResolveActionTarget_486E60, 0x00486E60, (CCharActionFsm* self, void*));
CSF_FUNC(int, __fastcall, CCharActionFsm_ResolveTargetFromSlot, 0x00485960, (CCharActionFsm* self, void*));

// Forwards the request to the base class, then, if an action FSM is attached and
// routing is not suppressed, notifies the resolved action target and the FSM's
// secondary target through their vtable slot 40 (offset 0xA0).
static int __fastcall CPlayerCharacter_ForwardRequestToActionTargets(CPlayerCharacter* self, void* /*edx*/, int a2, int a3, int a4, int a5)
{
    int forwardResult = C_00833908_ForwardRequest_48F2A0(reinterpret_cast<C_00833908*>(self), nullptr, a2, a3, a4, a5);

    uint32_t fsmValue = self->field_0044;
    if (fsmValue != 0 && !self->field_0228)
    {
        CCharActionFsm* fsm = reinterpret_cast<CCharActionFsm*>(static_cast<uintptr_t>(fsmValue));

        int actionTarget = CCharActionFsm_ResolveActionTarget_486E60(fsm, nullptr);
        uint32_t secondaryTarget = CSF_AT(uint32_t, fsm, 0x9C);

        if (actionTarget != 0)
        {
            if (actionTarget != CCharActionFsm_ResolveTargetFromSlot(fsm, nullptr))
            {
                void* targetObj = reinterpret_cast<void*>(static_cast<uintptr_t>(actionTarget));
                ((void(__fastcall*)(void*, void*, int, int, int))CSF_AT(void**, targetObj, 0)[160 / 4])(
                    targetObj, nullptr, a2, forwardResult, a5);
            }
        }

        if (secondaryTarget != 0)
        {
            void* secondaryObj = reinterpret_cast<void*>(static_cast<uintptr_t>(secondaryTarget));
            ((void(__fastcall*)(void*, void*, int, int, int))CSF_AT(void**, secondaryObj, 0)[160 / 4])(
                secondaryObj, nullptr, a2, forwardResult, a5);
        }
    }

    return forwardResult;
}

HOOK(0x495660, CPlayerCharacter_ForwardRequestToActionTargets, "auto");
