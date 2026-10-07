#include <cstdint>
#include "globals.h"
#include "hook.h"

// Global dword read by the getter (address 0x8B055C referenced by the original).
CSF_GLOBAL(int, g_Value8B055C, 0x8B055C);

// Plain getter: returns the current value of the global at 0x8B055C.
static int __cdecl GetValue8B055C()
{
    return g_Value8B055C;
}

HOOK(0x4F9660, GetValue8B055C, "auto");
