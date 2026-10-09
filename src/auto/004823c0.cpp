#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_004950e0_GetField398, 0x004950E0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_498F50, 0x00498F50, (void* self, void* edx, int a2, char a3));
CSF_FUNC(int, __fastcall, CCharWeaponFsm_InvokeVirtualSlot71With12, 0x00480210, (CCharWeaponFsm* self, void*));

static int __fastcall CCharActionFsm_ResolveWeaponState(CCharActionFsm* self, void* /*edx*/)
{
    if (C_004950e0_GetField398(reinterpret_cast<void*>(self->field_0008), nullptr))
    {
        self->vSlot1E8();
        return self->vSlot134(0, 0);
    }

    sub_498F50(reinterpret_cast<void*>(self->field_0008), nullptr, 512, 1);
    return CCharWeaponFsm_InvokeVirtualSlot71With12(self, nullptr);
}

HOOK(0x004823C0, CCharActionFsm_ResolveWeaponState, "auto");
