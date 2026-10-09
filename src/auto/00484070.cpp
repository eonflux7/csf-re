#include <cstddef>
#include "globals.h"
#include "hook.h"

// Allocates a buffer of twice the requested size via the game's debug allocator.
CSF_FUNC(int, __cdecl, Mem_AllocDebug, 0x00690DC0, (std::size_t size, int flags, int tag));

static int __stdcall CharActionFsm_AllocDoubledBuffer(int requestedSize, int unused)
{
    (void)unused;
    std::size_t doubledSize = static_cast<std::size_t>(requestedSize) * 2;
    return Mem_AllocDebug(doubledSize, 0, 0);
}

HOOK(0x484070, CharActionFsm_AllocDoubledBuffer, "auto");
