#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

using ThiscallNoArgFn = void(__fastcall*)(void* self, void* edx);

// Original: 0x480C60, __thiscall, ret 4 (one stack arg: char a2).
// Runs the attack-finish sequence for the weapon FSM when it is in state 4.
static void __fastcall CCharWeaponFsm_HandleAttackFinish(CCharWeaponFsm* self, void* /*edx*/, char a2)
{
    if (self->field_000c != 4)
        return;

    if (!a2)
    {
        // Notify the owner's sub-object (vtable slot 0x60) before the state change.
        void* owner = reinterpret_cast<void*>(self->field_0008);
        void* ownerSub = CSF_AT(void*, owner, 0x48);
        ThiscallNoArgFn notifyFn = reinterpret_cast<ThiscallNoArgFn>(CSF_AT(void**, ownerSub, 0)[0x60 / 4]);
        notifyFn(ownerSub, nullptr);
    }

    self->vSlot11C(0);

    // The returned object's type is not declared; dispatch through its vtable slot 0xA4.
    uint32_t target = static_cast<uint32_t>(self->vSlotD8());
    void* targetObj = reinterpret_cast<void*>(target);
    ThiscallNoArgFn targetFn = reinterpret_cast<ThiscallNoArgFn>(CSF_AT(void**, targetObj, 0)[0xA4 / 4]);
    targetFn(targetObj, nullptr);

    self->vSlot134(0, 0);
}

HOOK(0x480C60, CCharWeaponFsm_HandleAttackFinish, "auto");
