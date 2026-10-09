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

CSF_FUNC(int, __fastcall, C_004950e0_GetField398, 0x004950E0, (void* self, void*));
CSF_FUNC(int, __fastcall, sub_494E90, 0x00494E90, (void* self, void* edx, int a4));

// Cancels the active action if the linked object reports a pending state.
// Original: 0x004834C0, __thiscall, ret 0.
static int __fastcall CCharActionFsm_CancelActiveAction(CCharActionFsm* self, void* /*edx*/)
{
    int result = C_004950e0_GetField398(reinterpret_cast<C_00833ee8*>(self->field_0008), nullptr);
    if (result)
    {
        sub_494E90(reinterpret_cast<C_00833ee8*>(self->field_0008), nullptr, 0);
        return self->vBeginWeaponStateChange(2, 0);
    }
    return result;
}

HOOK(0x004834C0, CCharActionFsm_CancelActiveAction, "auto");
