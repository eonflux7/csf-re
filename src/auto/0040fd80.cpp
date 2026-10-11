#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Thin wrapper: game heap debug allocator, cdecl, pops nothing itself.
CSF_FUNC(int, __cdecl, Mem_AllocDebug, 0x00690DC0, (std::size_t size, int flags, int tag));

// Original: int __stdcall sub_40FD80(size_t a1, int a2)
// Forwards a1 as the allocation size with flags=0 and tag=0; a2 is never read.
static int __stdcall Heap_AllocateZeroTag(std::size_t size, int /*unusedArg*/) {
    return Mem_AllocDebug(size, 0, 0);
}

HOOK(0x0040FD80, Heap_AllocateZeroTag, "auto");
