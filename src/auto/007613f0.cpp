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

// Table of error description strings for the app_config module, indexed by error id.
CSF_GLOBAL(char*, g_psz_ESTATICO_app_config_3, 0x008A96A4);

static char* __cdecl AiAlert_GetErrorDescription(int errorIndex)
{
    return (&g_psz_ESTATICO_app_config_3)[errorIndex];
}

HOOK(0x007613F0, AiAlert_GetErrorDescription, "auto");
