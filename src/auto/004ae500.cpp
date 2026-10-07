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

// 0x004AE500 (6 bytes) - returns the process name string "TPrcLanzarGranada".
static const char* __cdecl GetProcesoName_TPrcLanzarGranada()
{
    return CSF_STR(0x00839D58, "TPrcLanzarGranada");
}

HOOK(0x004AE500, GetProcesoName_TPrcLanzarGranada, "auto");
