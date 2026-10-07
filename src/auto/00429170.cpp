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

// Float constant stored at 0x0082E05C (= 0.35f). The original loads it with
// `fld dword ptr [0x82e05c]` and returns it as a double (x87 widens float->double).
CSF_GLOBAL(float, g_FloatConstant_82e05c, 0x0082E05C);

// 0x00429170: returns the constant stored at 0x0082E05C as a double.
static double __cdecl GetFloatConstant()
{
    return static_cast<double>(g_FloatConstant_82e05c);
}

HOOK(0x429170, GetFloatConstant, "auto");
