#include <cstdint>
#include "globals.h"
#include "hook.h"

// Plain global variable returned as-is by the original accessor.
CSF_GLOBAL(int32_t, g_Value_8B0574, 0x8B0574);

// 0x518980: __cdecl, no arguments, returns dword_8B0574 (member/state accessor).
static int __cdecl GetValue_8B0574()
{
    return g_Value_8B0574;
}

HOOK(0x518980, GetValue_8B0574, "auto");
