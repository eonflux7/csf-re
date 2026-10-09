#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Asks the weapon's owner whether the weapon is ready. If it is not, requests state 9
// (WAIT_ARMA_READY) through the FSM's virtual state-change slot. Returns 1 when ready.
static int __fastcall CCharWeaponFsm_EnsureWeaponReadyOrRequestState(CCharWeaponFsm* self, void* /*edx*/, int /*a2*/)
{
    // Weapon handle comes from vSlotD8; its vtbl[42] (slot 0xA8) yields the owner object.
    void* weaponHandle = reinterpret_cast<void*>(static_cast<uintptr_t>(self->vSlotD8()));
    void* owner = ((void* (__fastcall*)(void*, void*))CSF_AT(void**, weaponHandle, 0)[42])(weaponHandle, nullptr);

    int result = 0;
    bool isReady = false;

    // owner vtbl[19] (slot 0x4C): first check, then fetch the sub-state and query its vtbl[7] (slot 0x1C).
    if (((int (__fastcall*)(void*, void*))CSF_AT(void**, owner, 0)[19])(owner, nullptr) != 0) {
        void* ownerState = ((void* (__fastcall*)(void*, void*))CSF_AT(void**, owner, 0)[19])(owner, nullptr);
        isReady = ((bool (__fastcall*)(void*, void*))CSF_AT(void**, ownerState, 0)[7])(ownerState, nullptr);
        result = isReady ? 1 : 0;
    }

    if (!isReady) {
        // owner vtbl[30] (slot 0x78) yields another sub-object; its vtbl[13] (slot 0x34) decides.
        void* alternateState = ((void* (__fastcall*)(void*, void*))CSF_AT(void**, owner, 0)[30])(owner, nullptr);
        bool alternateOk = ((bool (__fastcall*)(void*, void*))CSF_AT(void**, alternateState, 0)[13])(alternateState, nullptr);
        result = alternateOk ? 1 : 0;
        if (!alternateOk) {
            return self->vSlot134(9, 0);
        }
    }

    return result;
}

HOOK(0x0047FD80, CCharWeaponFsm_EnsureWeaponReadyOrRequestState, "auto");
