#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"

// Callee bindings (by address).
CSF_FUNC(bool, __fastcall, ActionTargetIsUsable, 0x004781E0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_004950e0_GetField398, 0x004950E0, (void* self, void*));

// Checks whether the character action FSM may start an action. When a2 is zero,
// also requires no pending flag, a valid current action target, and the
// GetField398 query on the owner to return zero.
static bool __fastcall CharActionFsm_CanStartAction(void* self, void* /*edx*/, char a2) {
    bool result = false;

    // Virtual slot 144 (0x90): fsm reports being active.
    bool isActive = ((bool(__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[36])(self, nullptr);
    if (isActive || CSF_AT(int, self, 0x0C) == 4) {
        int state = CSF_AT(int, self, 0x0C);
        if (state != 1 && state != 11 && state != 7) {
            result = true;
        }
    }

    if (!a2) {
        result = result
              && CSF_AT(int, self, 0x9C) == 0
              && ((void* (__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[55])(self, nullptr) != nullptr
              && ActionTargetIsUsable(
                     ((void* (__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[55])(self, nullptr),
                     nullptr)
              && C_004950e0_GetField398(CSF_AT(void*, self, 0x08), nullptr) == 0;
    }

    return result;
}

HOOK(0x483050, CharActionFsm_CanStartAction, "auto");
