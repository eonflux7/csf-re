#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(char*, __stdcall, sub_480740, 0x00480740, (int a1));

// Maps a CCharActionFsm state id (10..18) to its STATE_* name string.
// Unknown ids are delegated to the generic lookup at 0x480740.
static const char* __stdcall CharActionFsm_GetStateName(int stateId)
{
    const char* result;

    switch (stateId)
    {
    case 10:
        result = CSF_STR(0x00833498, "STATE_CONTEXTUAL");
        break;
    case 11:
        result = CSF_STR(0x008334BC, "STATE_CHANGE_ITEM");
        break;
    case 12:
        result = CSF_STR(0x008334AC, "STATE_USAR_ITEM");
        break;
    case 13:
        result = CSF_STR(0x00833488, "STATE_START_AIM");
        break;
    case 14:
        result = CSF_STR(0x00833478, "STATE_STOP_AIM");
        break;
    case 18:
        result = CSF_STR(0x00833464, "STATE_PEDIR_AYUDA");
        break;
    default:
        result = sub_480740(stateId);
        break;
    }
    return result;
}

HOOK(0x482A60, CharActionFsm_GetStateName, "auto");
