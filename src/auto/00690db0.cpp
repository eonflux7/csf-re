#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Global dword at 0x008BAD64, handed out by address.
CSF_GLOBAL(int32_t, g_dword_8BAD64, 0x8BAD64);

// Returns the address of the global dword at 0x008BAD64.
static int32_t* __cdecl GetGlobal8BAD64()
{
    return &g_dword_8BAD64;
}

HOOK(0x690DB0, GetGlobal8BAD64, "auto");
