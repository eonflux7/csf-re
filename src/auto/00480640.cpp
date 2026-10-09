#include <cstdint>
#include "globals.h"
#include <cstddef>

#include "hook.h"
#include "types/auto_classes.h"

// Original: 0x00480640, __thiscall, ret 0.
// Fetches a sub-object from vSlotD8, invokes its slot 0x84 method with no
// arguments (result unused), then forwards to vSlot11C with the argument 15.
static int __fastcall CCharWeaponFsm_RefreshWeaponStateAndSetMode15(CCharWeaponFsm* self, void* /*edx*/)
{
    int subObjectRaw = self->vGetIndexedEntryOrZero();
    void* subObject = reinterpret_cast<void*>(static_cast<intptr_t>(subObjectRaw));

    // Type of subObject is unknown here: dispatch through its vtable at slot 0x84 / 4.
    ((void(__fastcall*)(void*, void*))CSF_AT(void**, subObject, 0)[0x84 / 4])(subObject, nullptr);

    return self->vUpdateWeaponSelection(15);
}

HOOK(0x00480640, CCharWeaponFsm_RefreshWeaponStateAndSetMode15, "auto");
