#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_GLOBAL(int, g_Value_8B0564, 0x008B0564);

// 0x004FBDE0: returns the global dword at 0x008B0564.
static int __cdecl GetGlobalValue_4FBDE0()
{
    return g_Value_8B0564;
}

// LTCG callers (0x442BB0) keep a value in ecx across the call: the original never writes ecx.
HOOK_KEEP_ECX(0x004FBDE0, GetGlobalValue_4FBDE0, 0, "auto");
