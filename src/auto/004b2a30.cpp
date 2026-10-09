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

// LTCG callers (0x6525A0, 0x653180, 0x669760) keep a value in ecx across the call: the original never writes ecx.
HOOK_KEEP_ECX(0x4B2A30, ReturnZero, 0, "auto");
