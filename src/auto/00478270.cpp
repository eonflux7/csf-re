// Reimplementation of sub_478270 (Commandos: Strike Force)
// Returns the constant 50. The single stack argument is ignored.

#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// 0x00478270: __stdcall int sub_478270(int)
// Disassembly:
//   00478270  mov eax, 0x32
//   00478275  ret 4
static int __stdcall C_008317c8_GetConstant50(int /*a1*/)
{
    return 50;
}

HOOK(0x00478270, C_008317c8_GetConstant50, "auto");
