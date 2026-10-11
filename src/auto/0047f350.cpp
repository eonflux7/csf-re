#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original base-class routine (C_00832eec chain); called with this + two stack args, ret 8.
CSF_FUNC(void, __fastcall, AiAttackTask_PrepareTarget, 0x0047EDF0, (void* self, void* edx, uint32_t* a2, float a3));

// 0x47F350: forwards (a2, a3) to the base routine, then clears the byte at +0x2C.
static void __fastcall C_00832f18_InitAndClearFlag(C_00832f18* self, void* /*edx*/, uint32_t* a2, float a3)
{
    AiAttackTask_PrepareTarget(self, nullptr, a2, a3);
    self->field_002c = 0;
}

HOOK(0x0047F350, C_00832f18_InitAndClearFlag, "auto");
