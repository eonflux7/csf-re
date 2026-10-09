#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Forwards the state-object's sub-object address (this+0x3C) to the object returned
// by the virtual getter at slot 0xD8, via that object's vtable slot 0x40.
static void __fastcall CCharWeaponFsm_NotifyWeaponTargetIfBound(CCharWeaponFsm* self, void* /*edx*/)
{
    // Skip when the FSM has no bound target (field_0024 == -1).
    if (self->field_0024 == 0xFFFFFFFFu)
        return;

    // Virtual getter at slot 0xD8 (= 54) returns the bound target object.
    void* target = reinterpret_cast<void*>(self->vGetIndexedEntryOrZero());

    // Address of the embedded sub-object at +0x3C (not a loaded pointer).
    uint32_t* subObject = reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(self) + 0x3C);

    // Virtual call at slot 0x40 (= 16) on the target, with the sub-object as argument.
    ((void(__fastcall*)(void*, void*, uint32_t*))CSF_AT(void**, target, 0)[0x40 / 4])(target, nullptr, subObject);
}

HOOK(0x480B60, CCharWeaponFsm_NotifyWeaponTargetIfBound, "auto");
