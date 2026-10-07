#include <cstdint>
#include "globals.h"
#include "hook.h"

// Global value read by this accessor; true meaning unknown.
CSF_GLOBAL(int, g_Value8BC1EC, 0x8BC1EC);

// Returns the dword stored at 0x8BC1EC.
static int __cdecl GetGlobalValue8BC1EC()
{
    return g_Value8BC1EC;
}

HOOK(0x782600, GetGlobalValue8BC1EC, "auto");
