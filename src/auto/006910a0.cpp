#include <cstdint>
#include "globals.h"
#include "hook.h"

// The original stores the incoming argument into a single dword global at 0x8BAD60
// and returns the same value unchanged.
CSF_GLOBAL(int32_t, g_Value8BAD60, 0x008BAD60);

static int __cdecl SetGlobalValue8BAD60(int32_t value)
{
    g_Value8BAD60 = value;
    return value;
}

HOOK(0x006910A0, SetGlobalValue8BAD60, "auto");
