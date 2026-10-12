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

// Returns the localized name of an action-detail entry: indexes the
// g_psz_ANDAR string-pointer table by the first (and only used) argument.
static char* __cdecl GetActionDetailName(int actionDetailIndex, int unusedArg2, int unusedArg3) {
    CSF_GLOBAL(const char*, g_psz_ANDAR, 0x008A9648);
    return const_cast<char*>(reinterpret_cast<const char*>((&g_psz_ANDAR)[actionDetailIndex]));
}

HOOK(0x007613B0, GetActionDetailName, "auto");
