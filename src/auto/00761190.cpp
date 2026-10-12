#include <cstdint>
#include "hook.h"

// Returns nonzero when the sub-object type id is in the set the original accepts:
// every id from 0 through 0x14 (20), plus 0x1E (30).
static int __cdecl AiStatic_IsSubObjectUsable(unsigned int a1)
{
    return a1 <= 0x14 || a1 == 0x1e;
}

HOOK_KEEP_ECX_CDECL(0x761190, AiStatic_IsSubObjectUsable, 1, "app_config");
