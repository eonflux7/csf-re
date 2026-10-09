#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Tries to dispatch the weapon action: if the state object's flag 0x40 (at +0x2E0)
// is clear and its helper at +0x48 answers true to the predicate in vtable slot 4,
// it invokes the helper's action in vtable slot 8.
static int __fastcall CCharActionFsm_TryDispatchWeaponAction(CCharActionFsm* self, void* /*edx*/)
{
    void* stateObj = reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0008));
    int result = static_cast<int>(self->field_0008);

    if ((CSF_AT(uint8_t, stateObj, 0x2E0) & 0x40) == 0)
    {
        void* helperObj = reinterpret_cast<void*>(static_cast<uintptr_t>(CSF_AT(uint32_t, stateObj, 0x48)));
        if (helperObj != nullptr)
        {
            // Predicate: vtable slot 0x10 / 4 = 4, __thiscall(helper, this).
            using PredicateFn = int(__fastcall*)(void* self, void* edx, CCharActionFsm* arg);
            PredicateFn predicate = reinterpret_cast<PredicateFn>(CSF_AT(void**, helperObj, 0)[4]);
            result = predicate(helperObj, nullptr, self);

            if (static_cast<char>(result) != 0)
            {
                // Action: re-read the state object and its helper, vtable slot 0x20 / 4 = 8.
                void* actionObj = reinterpret_cast<void*>(static_cast<uintptr_t>(
                    CSF_AT(uint32_t, reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0008)), 0x48)));
                using ActionFn = int(__fastcall*)(void* self, void* edx, CCharActionFsm* arg);
                ActionFn action = reinterpret_cast<ActionFn>(CSF_AT(void**, actionObj, 0)[8]);
                return action(actionObj, nullptr, self);
            }
        }
    }
    return result;
}

HOOK(0x482370, CCharActionFsm_TryDispatchWeaponAction, "auto");
