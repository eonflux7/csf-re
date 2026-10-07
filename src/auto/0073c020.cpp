#include <cstdint>
#include "globals.h"
#include "hook.h"

// Global single dword at 0x008BC098 (probably a script/global variable slot or cached count).
CSF_GLOBAL(uint32_t, g_dword_8BC098, 0x8BC098);

// 0x0073C020 - trivial getter returning the global dword.
static int __cdecl GetGlobalDword_73C020()
{
    return static_cast<int>(g_dword_8BC098);
}

HOOK(0x0073C020, GetGlobalDword_73C020, "auto");
