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

// Mem_FreeDebug, bound at the address the original calls.
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// Releases a block through the debug heap free routine and returns its result.
// The first argument (a1) is not used by the original and is ignored.
static int __cdecl ReleaseDebugBlock(int a1, void* Block)
{
    (void)a1;
    return Mem_FreeDebug(Block);
}

HOOK(0x699DC0, ReleaseDebugBlock, "auto");
