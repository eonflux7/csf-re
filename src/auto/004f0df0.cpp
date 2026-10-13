#include "globals.h"
#include "hook.h"

// Table of const char* strings starting at 0x8998A4, indexed by a1.
CSF_GLOBAL(char*, g_psz_DISTRACCION_8998A4, 0x8998A4);

static char* __cdecl SceneLightsObjects_GetDistraccionString(int index)
{
    return (&g_psz_DISTRACCION_8998A4)[index];
}

HOOK(0x4F0DF0, SceneLightsObjects_GetDistraccionString, "auto");
