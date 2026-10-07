#include <cstdint>
#include "globals.h"
#include "hook.h"

// Global integer at 0x008B0688, returned verbatim by the original function.
CSF_GLOBAL(int, g_Int8B0688, 0x8B0688);

// 0x00550CC0 (6 bytes, `ret`) - returns the value of the global dword at 0x008B0688.
static int __cdecl GetGlobal8B0688()
{
    return g_Int8B0688;
}

HOOK(0x550CC0, GetGlobal8B0688, "auto");
