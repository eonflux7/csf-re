#include <cstdint>
#include "globals.h"
#include "hook.h"

// Global flag/pointer-presence value tested by this predicate.
CSF_GLOBAL(int32_t, g_Value_8afc78, 0x8AFC78);

// Returns non-zero when the global at 0x8AFC78 holds a non-zero value.
static int __cdecl IsGlobal8AFC78NonZero()
{
    return g_Value_8afc78 != 0;
}

HOOK(0x42D6E0, IsGlobal8AFC78NonZero, "auto");
