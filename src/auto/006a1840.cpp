#include <cstdint>
#include "globals.h"
#include "hook.h"

// Zlib-log module wrapper: forwards to the shared 8-argument init/setup routine
// with the fixed parameters 8, 13, 6, 0 inserted between the caller's arguments.
CSF_FUNC(int, __cdecl, ZlibLog_CoreSetup, 0x006A1640, (uint32_t* a1, int a2, int a3, int a4, int a5, unsigned int a6, uint8_t* a7, int a8));

static int __cdecl ZlibLog_InitWithDefaultParams(uint32_t* a1, int a2, uint8_t* a3, int a4)
{
    return ZlibLog_CoreSetup(a1, a2, 8, 13, 6, 0, a3, a4);
}

HOOK(0x6A1840, ZlibLog_InitWithDefaultParams, "auto");
