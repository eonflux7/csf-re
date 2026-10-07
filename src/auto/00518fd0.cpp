#include <cstdint>
#include "globals.h"
#include "hook.h"

// Simple dword accessor: returns *(int*)0x8B0584.
CSF_GLOBAL(int, g_GlobalValue_8B0584, 0x8B0584);

static int __cdecl GetGlobalValue_8B0584()
{
    return g_GlobalValue_8B0584;
}

HOOK(0x518FD0, GetGlobalValue_8B0584, "auto");
