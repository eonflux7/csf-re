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

// Returns the literal animation name used for the strafe blend.
static const char* __cdecl GetStrafeAnimName()
{
    return "TStrafe";
}

HOOK(0x4213D0, GetStrafeAnimName, "auto");
