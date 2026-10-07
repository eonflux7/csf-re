#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// 0x00523830 - returns the constant value 21.
static int __cdecl C_0083b3c0_GetConstant21()
{
    return 21;
}

HOOK(0x00523830, C_0083b3c0_GetConstant21, "auto");
