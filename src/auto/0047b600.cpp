#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CharActionFsm_IsWeaponActionBlocked, 0x00492BD0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CharTask_LookupByOffset, 0x004DB440, (void* self, void* edx, int a2));

// Forwards to CharTask_LookupByOffset(this, this->field_000c + 20) unless the
// owner's weapon action FSM reports the weapon action as blocked, in which case
// the FSM query result is returned unchanged.
static int __fastcall CharTask_LookupIfWeaponActionFree(C_00834fa8* self, void* /*edx*/, int /*a2*/)
{
    // this->field_0008 is a pointer to the owner; owner->+0x14 is the action FSM.
    uint8_t* owner = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x08));
    C_00833908* actionFsm = CSF_AT(C_00833908*, owner, 0x14);

    int result = CharActionFsm_IsWeaponActionBlocked(actionFsm, nullptr);
    if (!static_cast<uint8_t>(result)) {
        int lookupArg = static_cast<int>(CSF_AT(uint32_t, self, 0x0C) + 0x14);
        return CharTask_LookupByOffset(self, nullptr, lookupArg);
    }
    return result;
}

HOOK(0x0047B600, CharTask_LookupIfWeaponActionFree, "auto");
