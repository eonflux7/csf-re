// Target: 0x004AA290 (6 bytes) — trivial constant-returning function.
// Original returns the immediate 29 (0x1D) and pops no stack bytes (plain `ret`),
// so the calling convention is __cdecl.

#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// int sub_4AA290()
//   mov eax, 0x1D
//   ret
// Returns the constant 29. Purpose unknown (likely a type id / constant accessor).
static int __cdecl GetConstant29()
{
    return 29;
}

HOOK(0x4AA290, GetConstant29, "auto");
