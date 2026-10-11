#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callees (bound by address; same signatures as the packet's "bind as" lines).
CSF_FUNC(int, __fastcall, CharActionFsm_IsWeaponActionBlocked, 0x00492BD0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CharTask_LookupByOffset, 0x004DB440, (void* self, void* edx, int a2));

// Returns the weapon-blocked check result; if the action FSM reports the weapon
// action as blocked, looks up the task at (field_000C + 0x10C) and returns that.
static int __fastcall C_00834fa8_LookupOffsetIfWeaponBlocked(C_00834fa8* self, void* /*edx*/, int /*a2*/)
{
    // self->field_0008 is a pointer to the owning object; its +0x14 slot holds the action FSM.
    void* owner = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x08));
    void* actionFsm = CSF_AT(void*, owner, 0x14);

    int result = CharActionFsm_IsWeaponActionBlocked(actionFsm, nullptr);
    if ((uint8_t)result != 0) {
        int offsetArg = (int)(CSF_AT(uint32_t, self, 0x0C) + 0x10C);
        return CharTask_LookupByOffset(self, nullptr, offsetArg);
    }
    return result;
}

HOOK(0x004D7060, C_00834fa8_LookupOffsetIfWeaponBlocked, "auto");
