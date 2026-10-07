#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Global dword read by the accessor below (unknown meaning).
CSF_GLOBAL(int, g_Global_8B0680, 0x8B0680);

// 0x0054E520: returns the current value of the global dword at 0x8B0680.
static int __cdecl GetGlobalDword_8B0680()
{
    return g_Global_8B0680;
}

HOOK(0x54E520, GetGlobalDword_8B0680, "auto");
