#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Global read by the original function.
CSF_GLOBAL(int32_t, g_Value8B0558, 0x008B0558);

// Returns the global dword at 0x008B0558.
static int __cdecl GetGlobal8B0558()
{
    return g_Value8B0558;
}

HOOK(0x4F4920, GetGlobal8B0558, "auto");
