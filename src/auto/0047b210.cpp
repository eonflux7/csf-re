#include <cstdint>
#include "globals.h"
#include "hook.h"

static int __fastcall AttackTasks_SetMode(void* self, void* /*edx*/, int mode)
{
    CSF_AT(int, self, 0x2E8) = mode;
    return mode;
}

HOOK(0x47B210, AttackTasks_SetMode, "auto");
