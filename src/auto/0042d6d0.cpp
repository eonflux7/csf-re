#include <cstdint>
#include "globals.h"
#include "hook.h"

// Single global read at 0x008AFC78; purpose not evident from the caller set.
CSF_GLOBAL(int32_t, g_Value_8afc78, 0x008AFC78);

static int32_t __cdecl GetGlobal8afc78()
{
    return g_Value_8afc78;
}

HOOK(0x42D6D0, GetGlobal8afc78, "auto");
