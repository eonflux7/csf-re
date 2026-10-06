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

// Returns the static string "TAtacar" (address 0x00832E18 in the original).
static const char* __cdecl GetAttackActionName()
{
    return "TAtacar";
}

HOOK(0x004AA0F0, GetAttackActionName, "auto");
