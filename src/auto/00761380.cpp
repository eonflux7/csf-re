#include <cstdint>
#include "globals.h"
#include "hook.h"

// Returns the entry at index actionIndex of the g_psz_REPOSO string-pointer table.
// The original does no bounds check, so neither does this version.
static char* __cdecl GetRequestedActionName(int actionIndex) {
    return CSF_AT(char*, CSF_ADDR_DATA(0x008A8E18), actionIndex * 4);
}

HOOK(0x00761380, GetRequestedActionName, "auto");
