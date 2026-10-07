#include <cstdint>
#include "hook.h"
#include "globals.h"

CSF_GLOBAL(uint32_t, g_Value_8BC1E0, 0x8BC1E0);

// Returns the global dword stored at 0x008BC1E0.
static int __cdecl GetGlobalValue_8BC1E0()
{
    return g_Value_8BC1E0;
}

HOOK(0x756A10, GetGlobalValue_8BC1E0, "auto");
