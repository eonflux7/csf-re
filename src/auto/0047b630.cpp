#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// 0x47B630: __thiscall. Refreshes a readiness flag (byte at owner+0x2EC) from
// the sub-object at owner->+0x14->+0x48 via vtable slot 9 (arg 2). If the flag
// is set, calls vtable slot 35 on the owner's +0x14 object with (0, 0, -1, 0)
// and returns that call's result; otherwise returns the flag.
static char __fastcall AiAttackTasks_UpdateWeaponReadyFlag(void* self, void* /*edx*/) {
    void* owner = CSF_AT(void*, self, 0x8);
    void* ownerChild = CSF_AT(void*, owner, 0x14);
    void* readinessProvider = CSF_AT(void*, ownerChild, 0x48);

    char readyFlag = ((char(__fastcall*)(void*, void*, int))
        (CSF_AT(void**, readinessProvider, 0)[9]))(readinessProvider, nullptr, 2);
    CSF_AT(char, owner, 0x2EC) = readyFlag;

    owner = CSF_AT(void*, self, 0x8);
    if (CSF_AT(char, owner, 0x2EC) != 0) {
        void* notifyTarget = CSF_AT(void*, owner, 0x14);
        int notifyResult = ((int(__fastcall*)(void*, void*, int, int, int, int))
            (CSF_AT(void**, notifyTarget, 0)[35]))(notifyTarget, nullptr, 0, 0, -1, 0);
        return (char)notifyResult;
    }
    return CSF_AT(char, owner, 0x2EC);
}

HOOK(0x47B630, AiAttackTasks_UpdateWeaponReadyFlag, "auto");
