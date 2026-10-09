#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callee at 0x487400: __fastcall, self in ecx, one stack arg, ret 4.
CSF_FUNC(char, __fastcall, CCharActionFsm_TryPrepareTarget, 0x00487400, (void* self, void* edx, uint32_t* a2));

// 0x487610: if the target handle at +0x88 is accepted, flag the weapon (field_0008 + 0x2E4, bit 0x100),
// copy the target into +0xA4 and invoke the state-change virtual slot 0x134 with (12, 0).
static int __fastcall CCharActionFsm_TryBeginTargetedAction(CCharActionFsm* self, void* /*edx*/)
{
    const char targetAccepted = CCharActionFsm_TryPrepareTarget(
        self, nullptr, reinterpret_cast<uint32_t*>(self->field_0088));
    if (!targetAccepted)
        return targetAccepted;

    CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0008), 0x2E4) |= 0x100u;
    self->field_00a4 = self->field_0088;
    return self->vSlot134(12, 0);
}

HOOK(0x487610, CCharActionFsm_TryBeginTargetedAction, "auto");
