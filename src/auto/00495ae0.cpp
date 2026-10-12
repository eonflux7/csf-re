#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, CCharActionFsm_CanUseItem, 0x00487400, (CCharActionFsm* self, void* /*edx*/, uint32_t* item));

static char __fastcall C_00833b88_RequestAuxiliaryAction(C_00833b88* self, void* /*edx*/) {
    CCharActionFsm* fsm = reinterpret_cast<CCharActionFsm*>(self->field_0044);
    uint32_t* item = CSF_AT(uint32_t*, reinterpret_cast<void*>(self->field_0044), 0x88);

    char result = CCharActionFsm_CanUseItem(fsm, nullptr, item);
    if (result) {
        CSF_AT(uint32_t, self, 0x2E4) |= 0x20000u;
    }
    return result;
}

HOOK(0x00495AE0, C_00833b88_RequestAuxiliaryAction, "auto");
