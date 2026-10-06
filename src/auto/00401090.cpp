#include <cstdint>
#include "globals.h"
#include "hook.h"

// Global int consulted by this trivial getter. Purpose unknown from the packet;
// named after its address.
CSF_GLOBAL(int, g_Value_8afc60, 0x8AFC60);

// Returns the global dword stored at 0x008AFC60.
static int __cdecl GetGlobalValue_401090()
{
    return g_Value_8afc60;
}

HOOK(0x401090, GetGlobalValue_401090, "auto");
