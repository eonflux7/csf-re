#include <cstdint>
#include "globals.h"
#include "hook.h"

// Global dword at 0x008AFC7C, returned verbatim by the original function.
CSF_GLOBAL(int, g_Value8afc7c, 0x8AFC7C);

// 0x0043E010 (6 bytes): plain __cdecl getter, no arguments, ret 0.
static int __cdecl GetGlobal8afc7c()
{
    return g_Value8afc7c;
}

HOOK(0x43E010, GetGlobal8afc7c, "auto");
