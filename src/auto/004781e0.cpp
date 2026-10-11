#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_008319c0_GetSubObjectField74, 0x004760A0, (C_008319c0* self, void*));

static bool __fastcall C_008319c0_IsActionTargetUsable(C_008319c0* self, void* /*edx*/)
{
    return (C_008319c0_GetSubObjectField74(self, nullptr) == 1 ||
            C_008319c0_GetSubObjectField74(self, nullptr) == 2) &&
           CSF_AT(int, self, 0x70) == 0;
}

HOOK(0x004781E0, C_008319c0_IsActionTargetUsable, "auto");
