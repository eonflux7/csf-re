#include <cstdint>
#include "globals.h"
#include "hook.h"

// Global value returned verbatim by the target function.
CSF_GLOBAL(int, g_Value8B0554, 0x8B0554);

// Returns the value of the global at 0x8B0554. No side effects.
static int __cdecl GetValue8B0554()
{
    return g_Value8B0554;
}

// LTCG callers (0x462D00) keep a value in ecx across the call: the original never writes ecx.
HOOK_KEEP_ECX(0x4E8C50, GetValue8B0554, 0, "auto");
