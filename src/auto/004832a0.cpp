#include "hook.h"
#include "globals.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_008319c0_GetSubObjectField74, 0x004760A0, (C_008319c0* self, void* /*edx*/));

// Returns false when the FSM is in the -1 state or the attached sub-object is in a
// blocking state (74 == 31, 27 or 23). Otherwise requests state 1 and returns true.
static bool __fastcall CCharActionFsm_TryRequestStateOne(CCharActionFsm* self, void* /*edx*/)
{
    if (self->field_0028 == 0xFFFFFFFFu)
        return false;

    C_008319c0* subObject = reinterpret_cast<C_008319c0*>(self->field_009c);
    if (subObject
        && (C_008319c0_GetSubObjectField74(subObject, nullptr) == 31
            || C_008319c0_GetSubObjectField74(subObject, nullptr) == 27
            || C_008319c0_GetSubObjectField74(subObject, nullptr) == 23))
    {
        return false;
    }

    self->vSlot134(1, 0);
    return true;
}

HOOK(0x4832A0, CCharActionFsm_TryRequestStateOne, "auto");
