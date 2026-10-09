#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Gets the object returned by vSlotD8, asks its slot 0x2C predicate, and if true
// issues vSlot134 with the weapon's stored value at +0x64 and 0.
static int __fastcall CCharWeaponFsm_TryIssueWeaponAction(CCharWeaponFsm* self, void* /*edx*/, int /*unusedArg*/) {
    void* target = reinterpret_cast<void*>(static_cast<uintptr_t>(self->vSlotD8()));

    // Slot 0x2C / 4 = index 11 on the target's vtable; ecx = target, edx unused.
    int predicateResult = ((int(__fastcall*)(void*, void*))CSF_AT(void**, target, 0)[11])(target, nullptr);

    if (static_cast<uint8_t>(predicateResult) != 0) {
        return self->vSlot134(static_cast<int>(self->field_0064), 0);
    }
    return predicateResult;
}

HOOK(0x47FCE0, CCharWeaponFsm_TryIssueWeaponAction, "auto");
