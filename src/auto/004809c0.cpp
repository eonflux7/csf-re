#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Sets the weapon FSM state index. Does nothing when the requested state is -1
// or already current; otherwise stores it and notifies the object through
// vtable slot 0x1F4 (state-changed hook), returning that call's result.
static int __fastcall CCharWeaponFsm_SetWeaponStateIfChanged(CCharWeaponFsm* self, void* /*edx*/, int requestedState)
{
    int result = requestedState;
    if (requestedState != -1 && requestedState != static_cast<int>(self->field_0028))
    {
        self->field_0028 = static_cast<uint32_t>(requestedState);
        // Slot 0x1F4 / 4 = 125: the typed wrapper vSlot1F4 is void, but the original
        // returns the callee's eax, so the raw slot call is kept to preserve the value.
        return ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, self, 0)[0x1F4 / 4])(self, nullptr, 1);
    }
    return result;
}

HOOK(0x4809C0, CCharWeaponFsm_SetWeaponStateIfChanged, "auto");
