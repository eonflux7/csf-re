#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Original 0x00690E60: debug-aware free used by the game heap.
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// Original 0x00691110 (__cdecl, ret 0): the original tail-jumps to Mem_FreeDebug,
// forwarding its single stack argument unchanged and returning its result.
static int __cdecl CoreMemory_Free(void* block)
{
    return Mem_FreeDebug(block);
}

HOOK(0x00691110, CoreMemory_Free, "auto");
