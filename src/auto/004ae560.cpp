// Function at 0x004AE560 — returns the constant 23.
// Disassembly:
//   004ae560  mov eax, 0x17
//   004ae565  ret
// Plain __cdecl function taking no arguments (callee pops 0 stack bytes).

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

static int __cdecl GetConstant23()
{
    return 23;
}

HOOK(0x4AE560, GetConstant23, "auto");
