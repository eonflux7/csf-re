#include <cstdint>
#include "globals.h"
#include "hook.h"

// Global value at 0x008BC1E8, returned verbatim by the target.
CSF_GLOBAL(int, g_Value_8BC1E8, 0x8BC1E8);

// Returns the current value of the global int at 0x008BC1E8.
static int __cdecl GetGlobalValue_768E70()
{
  return g_Value_8BC1E8;
}

HOOK(0x768E70, GetGlobalValue_768E70, "auto");
