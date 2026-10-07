#include "globals.h"
#include "hook.h"

// The global dword whose address this accessor hands out. Type is unknown;
// the original only ever exposes its address, so we keep it as an int.
CSF_GLOBAL(int, g_Value_8BAFA4, 0x8BAFA4);

// Returns the address of the global at 0x8BAFA4.
static int* __cdecl GetGlobalIntPtr()
{
    return &g_Value_8BAFA4;
}

HOOK(0x69C580, GetGlobalIntPtr, "auto");
