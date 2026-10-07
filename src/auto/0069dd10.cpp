#include <cstdint>

#include "globals.h"
#include "hook.h"

// Global data at 0x008BB408; these functions hand out a pointer to it.
CSF_GLOBAL(int32_t, g_8BB408, 0x8BB408);

// Returns the address of the dword at 0x008BB408 (accessor for the global).
static int32_t* __cdecl Get_8BB408()
{
    return &g_8BB408;
}

HOOK(0x69DD10, Get_8BB408, "auto");
