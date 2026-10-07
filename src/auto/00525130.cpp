#include <cstdint>
#include "globals.h"
#include "hook.h"

// Single global dword read by the original function.
CSF_GLOBAL(int, g_008b0678, 0x008B0678);

// 0x00525130: trivial accessor that returns the global dword at 0x008B0678.
// No `this`, no stack arguments (ret 0) -> __cdecl.
static int __cdecl Get_008b0678()
{
    return g_008b0678;
}

HOOK(0x00525130, Get_008b0678, "auto");
