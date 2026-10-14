#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Asks the weapon's owner whether the weapon is ready. If it is not, requests state 9
// (WAIT_ARMA_READY) through the FSM's virtual state-change slot. When ready, returns the callee's full
// eax (only al is tested by the original), not a normalized 0/1.
static int __fastcall CCharWeaponFsm_EnsureWeaponReadyOrRequestState(CCharWeaponFsm* self, void* /*edx*/, int /*a2*/)
{
    // Weapon handle comes from vSlotD8; its vtbl[42] (slot 0xA8) yields the owner object.
    void* weaponHandle = reinterpret_cast<void*>(static_cast<uintptr_t>(self->vGetIndexedEntryOrZero()));
    void* owner = ((void* (__fastcall*)(void*, void*))CSF_AT(void**, weaponHandle, 0)[42])(weaponHandle, nullptr);

    // owner vtbl[19] (slot 0x4C): first check, then fetch the sub-state and query its vtbl[7] (slot 0x1C).
    // The original tests only al (test al, al) but returns the callee's whole eax, so the callees are
    // bound as returning int and the full value is passed through.
    if (((int (__fastcall*)(void*, void*))CSF_AT(void**, owner, 0)[19])(owner, nullptr) != 0) {
        void* ownerState = ((void* (__fastcall*)(void*, void*))CSF_AT(void**, owner, 0)[19])(owner, nullptr);
        int ready = ((int (__fastcall*)(void*, void*))CSF_AT(void**, ownerState, 0)[7])(ownerState, nullptr);
        if (static_cast<uint8_t>(ready) != 0) {
            return ready;
        }
    }

    // owner vtbl[30] (slot 0x78) yields another sub-object; its vtbl[13] (slot 0x34) decides.
    void* alternateState = ((void* (__fastcall*)(void*, void*))CSF_AT(void**, owner, 0)[30])(owner, nullptr);
    int alternateOk = ((int (__fastcall*)(void*, void*))CSF_AT(void**, alternateState, 0)[13])(alternateState, nullptr);
    if (static_cast<uint8_t>(alternateOk) != 0) {
        return alternateOk;
    }
    return self->vBeginWeaponStateChange(9, 0);
}

HOOK(0x0047FD80, CCharWeaponFsm_EnsureWeaponReadyOrRequestState, "auto");
