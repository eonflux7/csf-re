#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"

// Table of description strings for AI alerts, indexed by alert id.
CSF_GLOBAL(char*, g_psz_ESTATICO_app_config_4, 0x008A96B8);

static char* AiAlert_LookupDescription(int alertIndex)
{
    return (&g_psz_ESTATICO_app_config_4)[alertIndex];
}

HOOK(0x00761400, AiAlert_LookupDescription, "auto");
