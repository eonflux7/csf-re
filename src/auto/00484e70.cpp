#include "hook.h"
#include "globals.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, sub_4846D0, 0x004846D0, (void* self, void* edx));
CSF_FUNC(void, __fastcall, CCharWeaponFsm_ResetField0028, 0x00480200, (CCharWeaponFsm* self, void* edx));

static void __fastcall CCharActionFsm_ResetToIdle(CCharActionFsm* self, void* /*edx*/)
{
    sub_4846D0(self, nullptr);
    self->field_0094 = 0xFFFFFFFFu;
    CCharWeaponFsm_ResetField0028(self, nullptr);
}

HOOK(0x00484E70, CCharActionFsm_ResetToIdle, "auto");
