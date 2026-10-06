#include <cstdint>
#include "globals.h"
#include "hook.h"

// The game keeps the display name of the "throw grenade" action in a global
// character buffer at 0x83688C (not in .rdata), so we return its address.
CSF_GLOBAL(char, g_szLanzarGranadaName, 0x83688C);

// 0x004A9FA0 (ret 0) -- returns the name of the grenade-throw action.
static const char* __cdecl GetLanzarGranadaName()
{
    return &g_szLanzarGranadaName;
}

HOOK(0x4A9FA0, GetLanzarGranadaName, "auto");
