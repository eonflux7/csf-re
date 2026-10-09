#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Virtual slot 0x10 on the target object's vtable (slot 4): __thiscall bool(CCharWeaponFsm* other).
// Its result gates a second virtual call on `self`.
static void __fastcall CCharWeaponFsm_UpdateTargetAndNotify(CCharWeaponFsm* self, void* /*edx*/, int a2)
{
    if (self->m_stateCode == 9)
        return;

    void* argObj = reinterpret_cast<void*>(a2);
    if (CSF_AT(uint8_t, argObj, 4) == 0)
        return;
    if (CSF_AT(void*, argObj, 0) == static_cast<void*>(self))
        return;

    // owner = self->field_0008; target = owner->field_0048
    void* owner = reinterpret_cast<void*>(self->field_0008);
    void* target = reinterpret_cast<void*>(CSF_AT(uint32_t, owner, 0x48));

    // target's vtable slot 0x10 (index 4)
    auto checkFn = reinterpret_cast<bool (__fastcall*)(void*, void*, CCharWeaponFsm*)>(
        CSF_AT(void**, target, 0)[0x10 / 4]);
    if (checkFn(target, nullptr, self))
    {
        self->vBeginWeaponStateChange(0, 0);
    }
}

HOOK(0x480820, CCharWeaponFsm_UpdateTargetAndNotify, "auto");
