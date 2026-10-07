#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Global int returned verbatim by the target function.
CSF_GLOBAL(int, g_Value8BBB28, 0x008BBB28);

// 0x006F7820 (__cdecl, ret 0): simply returns the value of the global at 0x008BBB28.
static int __cdecl GetValue8BBB28()
{
    return g_Value8BBB28;
}

HOOK(0x6F7820, GetValue8BBB28, "auto");
