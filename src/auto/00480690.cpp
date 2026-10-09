#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Replaces 0x00480690 (CCharWeaponFsm, __thiscall, ret 0).
// Asks the object returned by vSlotD8 whether its virtual check (vtbl slot 0x2C / 4 = 11)
// passes; if it does, dispatches vSlot11C(20) on the weapon FSM and returns its result.
// Otherwise returns the raw check result.
static int __fastcall CCharWeaponFsm_DispatchWhenCheckPasses(CCharWeaponFsm* self, void* /*edx*/)
{
    // First virtual call: vSlotD8 gives the sub-object whose vtable we query.
    int checkedObjectValue = self->vSlotD8();
    void* checkedObject = reinterpret_cast<void*>(static_cast<intptr_t>(checkedObjectValue));

    // Second virtual call: slot 0x2C -> index 11, no arguments.
    int checkResult = ((int(__fastcall*)(void*, void*))CSF_AT(void**, checkedObject, 0)[11])(checkedObject, nullptr);

    // Original tests only the low byte (test al, al).
    if (static_cast<uint8_t>(checkResult) != 0)
    {
        return self->vSlot11C(20);
    }
    return checkResult;
}

HOOK(0x480690, CCharWeaponFsm_DispatchWhenCheckPasses, "auto");
