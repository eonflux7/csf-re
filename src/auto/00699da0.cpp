#include <cstddef>
#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, Mem_AllocDebug, 0x00690DC0, (std::size_t size, int flags, int tag));

// Allocates a block of count * elementSize bytes through the debug allocator (flags 0, tag 0).
static int __cdecl Mem_AllocArrayDebug(int /*unused*/, int elementSize, int count) {
    std::uint32_t totalBytes = static_cast<std::uint32_t>(count) * static_cast<std::uint32_t>(elementSize);
    return Mem_AllocDebug(static_cast<std::size_t>(totalBytes), 0, 0);
}

HOOK(0x00699DA0, Mem_AllocArrayDebug, "auto");
