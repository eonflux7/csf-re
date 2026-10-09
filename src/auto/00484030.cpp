#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Returns non-zero when the weapon-FSM state code at +0x10 is 40 or 41.
static int __fastcall CCharActionFsm_IsStateCodeFortyOrFortyOne(CCharActionFsm* self, void* /*edx*/)
{
    uint32_t stateCode = self->field_0010;
    return stateCode == 40 || stateCode == 41;
}

HOOK(0x484030, CCharActionFsm_IsStateCodeFortyOrFortyOne, "auto");
