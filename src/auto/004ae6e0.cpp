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

// 0x004AE6E0 (6 bytes) - returns the literal name of the "TFncReaccionExplosivos" type.
// Original:
//   mov eax, 0x83a0dc
//   ret
static const char* __cdecl FncReaccionExplosivos_GetName()
{
    return CSF_STR(0x83A0DC, "TFncReaccionExplosivos");
}

HOOK(0x4AE6E0, FncReaccionExplosivos_GetName, "auto");
