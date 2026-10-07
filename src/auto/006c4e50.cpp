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

// Float constant loaded by the original code.
// Contents at 0x008608A8 == 1.0e8f (0x4CBEBC20); value is re-read on every call.
CSF_GLOBAL(float, g_Value1e8, 0x8608A8);

// Returns the float constant as a double (x87: fld dword ptr [0x8608A8]; ret).
static double __cdecl Constant1e8()
{
    return g_Value1e8;
}

HOOK(0x6C4E50, Constant1e8, "auto");
