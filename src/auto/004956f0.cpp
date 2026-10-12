#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Bind as lines copied from the callee packet.
CSF_FUNC(int, __fastcall, CCharActionFsm_ResolveActionTarget_486E60, 0x00486E60, (CCharActionFsm* self, void*));
CSF_FUNC(int, __fastcall, C_00833908_ResetChildAndDispatch, 0x0048E550, (C_00833908* self, void*));

// Resets the base child and dispatches. If an action FSM is attached, resolves its target and
// calls the target's slot 0xA4 method. If the FSM's secondary object is set, forwards the call
// to that object's slot 0xA4 method as a tail jump with the caller's stack arguments untouched.
static hook::Tail __fastcall CPlayerCharacter_ResetAndForwardActionTail(void* self, const uint32_t* args) {
    (void)args;
    auto* character = reinterpret_cast<C_00833908*>(self);

    int result = C_00833908_ResetChildAndDispatch(character, nullptr);

    uint32_t fsmAddress = character->field_0044;
    if (fsmAddress == 0) {
        return hook::tail_ret(result);
    }

    auto* actionFsm = reinterpret_cast<CCharActionFsm*>(fsmAddress);
    result = CCharActionFsm_ResolveActionTarget_486E60(actionFsm, nullptr);

    // Secondary object is read before the vtable call, as in the original.
    uint32_t secondaryAddress = CSF_AT(uint32_t, actionFsm, 0x9C);

    if (result != 0) {
        void* targetObject = reinterpret_cast<void*>(static_cast<uintptr_t>(result));
        result = ((int (__fastcall*)(void*, void*))CSF_AT(void**, targetObject, 0)[0xA4 / 4])(targetObject, nullptr);
    }

    if (secondaryAddress != 0) {
        void* secondaryObject = reinterpret_cast<void*>(static_cast<uintptr_t>(secondaryAddress));
        return hook::tail_jump(secondaryObject, (*reinterpret_cast<void***>(secondaryObject))[0xA4 / 4]);
    }

    return hook::tail_ret(result);
}

HOOK_TAIL(0x004956F0, CPlayerCharacter_ResetAndForwardActionTail, "auto");
