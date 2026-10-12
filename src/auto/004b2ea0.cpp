#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, AiCoverAttackTasks_TestAttackPosition, 0x004B2E20, (void* self, void* edx, float* a2, int a3));

static char __fastcall S_004af750_TestStoredAttackPosition(
    S_004af750* self, void* /*edx*/, int argument)
{
    if (self->field_06c8 &&
        self->field_06c0 != UINT32_MAX &&
        self->field_06c4 != UINT32_MAX)
    {
        return AiCoverAttackTasks_TestAttackPosition(
            self, nullptr, &CSF_AT(float, self, 0x6B4), argument);
    }
    return 0;
}

HOOK(0x004B2EA0, S_004af750_TestStoredAttackPosition, "auto");
