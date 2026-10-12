#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, IsSubStateEnabled, 0x00760F90, (int a1));
CSF_FUNC(int, __cdecl, IsSubObjectFieldFlagSet, 0x00760FF0, (int a1));
CSF_FUNC(int, __cdecl, AiStatic_IsSubObjectUsable, 0x00761190, (unsigned int a1));

// Returns nonzero when the sub-object id is usable for the char action FSM:
// it is enabled, not flagged, not 0x20/9, and passes the usability check;
// id 8 is accepted regardless of the other checks.
static int __cdecl CharActionFsm_CheckSubObjectState(int a1)
{
    if (!IsSubStateEnabled(a1)
        && !IsSubObjectFieldFlagSet(a1)
        && a1 != 0x20
        && a1 != 9
        && AiStatic_IsSubObjectUsable((unsigned int)a1))
    {
        return 1;
    }
    return a1 == 8 ? 1 : 0;
}

HOOK(0x00761330, CharActionFsm_CheckSubObjectState, "auto");
