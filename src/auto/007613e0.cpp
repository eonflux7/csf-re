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

// Table of const char* string pointers indexed by a small integer (app_config section).
CSF_GLOBAL(const char**, g_psz_ESTATICO_app_config_2, 0x008A969C);

static char* __cdecl AiDistracted_QueryLinkedStatusText(int index)
{
    return reinterpret_cast<char*>(const_cast<char*>(g_psz_ESTATICO_app_config_2[index]));
}

HOOK(0x007613E0, AiDistracted_QueryLinkedStatusText, "auto");
