#include "hook.h"

// __cdecl, 2 stack args, ret 0. Returns a byte bool through al.
static bool SceneMissionSetup_GetObjectStateFlag(int objectType, int stateValue) {
    // Type 0 is a special case: only states 4, 5 and 6 qualify.
    if (objectType == 0) {
        return stateValue == 4 || stateValue == 6 || stateValue == 5;
    }

    switch (objectType) {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 18:
    case 19:
    case 25:
    case 30:
    case 31:
    case 33:
        return true;
    default:
        return false;
    }
}

HOOK(0x7559A0, SceneMissionSetup_GetObjectStateFlag, "auto");
