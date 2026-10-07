#include <cstdint>
#include "globals.h"
#include "hook.h"

// Global flag read (and tested for non-zero) by this predicate.
CSF_GLOBAL(int, g_Value8AFC84, 0x8AFC84);

// Returns non-zero if the global flag at 0x8AFC84 is set.
static int __cdecl IsGlobalFlagSet()
{
    return g_Value8AFC84 != 0;
}

HOOK(0x44ACF0, IsGlobalFlagSet, "auto");
