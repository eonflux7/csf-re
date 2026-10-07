#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Global dword at 0x8B0668, returned as-is. Type/usage unknown from this
// single accessor; kept as a signed 32-bit value (mov eax, [mem]).
CSF_GLOBAL(int, g_Value8B0668, 0x8B0668);

static int __cdecl GetDword8B0668()
{
    return g_Value8B0668;
}

HOOK(0x519270, GetDword8B0668, "auto");
