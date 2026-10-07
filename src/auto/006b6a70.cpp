#include <cstdint>
#include "hook.h"

// Returns the constant value 44 (0x2C).
// Original code: mov eax, 0x2C ; ret
static int __cdecl GetConstant44()
{
    return 44;
}

HOOK(0x006B6A70, GetConstant44, "auto");
