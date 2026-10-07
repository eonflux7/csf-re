#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Returns the size in bytes of a C_00860848 instance (0x34 = 52).
static int __cdecl C_00860848_GetObjectSize()
{
    return 52;
}

HOOK(0x006C3560, C_00860848_GetObjectSize, "auto");
