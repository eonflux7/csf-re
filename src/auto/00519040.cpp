#include <cstdint>
#include "globals.h"
#include "hook.h"

// Global dword read and returned verbatim by the original (no side effects).
CSF_GLOBAL(int, g_Value8B0588, 0x8B0588);

// 0x00519040: trivial getter returning the global dword at 0x008B0588.
static int __cdecl GetValue8B0588()
{
    return g_Value8B0588;
}

HOOK(0x519040, GetValue8B0588, "auto");
