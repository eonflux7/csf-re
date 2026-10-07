#include <cstdint>
#include "globals.h"
#include "hook.h"

// Constant float value living in read-only data (0x00831090 == FLT_MAX).
CSF_GLOBAL(float, g_FloatMax, 0x00831090);

// Returns the constant float max value, widened to double (fld dword -> st(0)).
static double GetFloatMaxConstant()
{
    return g_FloatMax;
}

HOOK(0x00527530, GetFloatMaxConstant, "auto");
