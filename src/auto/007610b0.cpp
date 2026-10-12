#include <cstdint>
#include "hook.h"

// Returns 1 when the sub-object type id is one of the four eligible ids (30, 12, 13, 20), otherwise 0.
static int __cdecl AppConfig_IsSubObjectEligible(int typeId) {
    return typeId == 30 || typeId == 12 || typeId == 13 || typeId == 20;
}

HOOK(0x007610B0, AppConfig_IsSubObjectEligible, "auto");
