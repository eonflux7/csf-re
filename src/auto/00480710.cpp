#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original: 0x00480710, __thiscall, ret 0.
// Gets a subobject through the virtual slot 0xD8, calls its slot 0x30 with arg 1,
// then returns the result of the self virtual slot 0x11C with arg 15.
static int __fastcall CCharWeaponFsm_SetSubobjectFlagThenQueryState15(CCharWeaponFsm* self, void* /*edx*/)
{
    // Slot 0xD8 returns the subobject pointer (Hex-Rays types it as int).
    void* subobject = reinterpret_cast<void*>(self->vSlotD8());

    // Virtual slot 0x30 on the subobject (index 12), __thiscall with one argument of 1.
    using SubobjectSetFlagFn = void(__fastcall*)(void* self, void* edx, int flag);
    SubobjectSetFlagFn setFlag = reinterpret_cast<SubobjectSetFlagFn>(CSF_AT(void**, subobject, 0)[0x30 / 4]);
    setFlag(subobject, nullptr, 1);

    // Return the result of the self virtual slot 0x11C with argument 15.
    return self->vSlot11C(15);
}

HOOK(0x00480710, CCharWeaponFsm_SetSubobjectFlagThenQueryState15, "auto");
