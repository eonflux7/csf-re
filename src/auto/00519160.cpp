#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Global value returned verbatim by the original function.
CSF_GLOBAL(int, g_Value_8B0660, 0x8B0660);

// 0x00519160: trivial getter that returns the global at 0x8B0660.
static int __cdecl GetGlobal8B0660()
{
    return g_Value_8B0660;
}

HOOK(0x519160, GetGlobal8B0660, "auto");
