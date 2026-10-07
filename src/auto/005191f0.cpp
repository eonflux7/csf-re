#include <cstdint>
#include "globals.h"
#include "hook.h"

// 0x008B0664: 32-bit global value returned verbatim by this accessor.
CSF_GLOBAL(int32_t, g_Value_008B0664, 0x8B0664);

// Returns the current contents of the global at 0x008B0664.
static int32_t __cdecl GetSharedValue()
{
    return g_Value_008B0664;
}

HOOK(0x5191F0, GetSharedValue, "auto");
