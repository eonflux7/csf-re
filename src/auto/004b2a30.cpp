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

// 0x004B2A30 - trivial constant stub: always returns 0.
// Disassembly: xor eax, eax ; ret
static int __cdecl ReturnZero()
{
    return 0;
}

HOOK(0x4B2A30, ReturnZero, "auto");
