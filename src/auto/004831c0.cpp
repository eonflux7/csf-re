#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_008319c0_GetSubObjectField74, 0x004760A0, (C_008319c0* self, void* edx));

// Returns true when the object is non-null and its sub-object field 74 is 31, 27 or 23.
static bool __stdcall CCharActionFsm_IsField74InReloadOrAttackSet(C_008319c0* a1)
{
    if (a1 == nullptr)
        return false;

    if (C_008319c0_GetSubObjectField74(a1, nullptr) == 31)
        return true;
    if (C_008319c0_GetSubObjectField74(a1, nullptr) == 27)
        return true;
    if (C_008319c0_GetSubObjectField74(a1, nullptr) == 23)
        return true;
    return false;
}

HOOK(0x004831C0, CCharActionFsm_IsField74InReloadOrAttackSet, "auto");
