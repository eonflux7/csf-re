#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, GetMemberValueD4, 0x00476090, (void* self, void*));

// Ticks the active weapon state. Skipped when the state is 9 or the sub-state
// is -1. State 4 checks readiness and the weapon sub-object, then fires the
// member-value check; any other state forwards to the virtual handler at +0x120.
static int __fastcall CCharWeaponFsm_UpdateActiveState(CCharWeaponFsm* self, void* /*edx*/, int a2) {
    int result = static_cast<int>(self->m_stateCode);
    if (result == 9 || static_cast<int>(self->field_0024) == -1) {
        return result;
    }

    if (result != 4) {
        return self->vDispatchRequest(reinterpret_cast<uint32_t*>(a2));
    }

    result = self->vHasUsableWeapon();
    if (!static_cast<unsigned char>(result)) {
        return result;
    }

    int weaponObjA = self->vGetIndexedEntryOrZero();
    void* weaponObj = reinterpret_cast<void*>(weaponObjA);
    char objReady = ((char (__fastcall*)(void*, void*))CSF_AT(void**, weaponObj, 0)[11])(weaponObj, nullptr);
    if (!objReady) {
        return static_cast<int>(objReady);
    }

    int weaponObjB = self->vGetIndexedEntryOrZero();
    result = GetMemberValueD4(reinterpret_cast<void*>(weaponObjB), nullptr);
    if (result == 1) {
        self->vHandleAttackFinish(0);
        return 0;
    }
    return result;
}

HOOK(0x00480860, CCharWeaponFsm_UpdateActiveState, "auto");
