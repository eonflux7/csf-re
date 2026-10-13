#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"

CSF_FUNC(void*, __cdecl, Mem_AllocTagged, 0x00691120, (uint32_t a1, uint16_t a2));

// Allocates count * elementSize bytes through the tagged game allocator and zeroes them.
// Returns nullptr when the allocator fails, without touching memory.
static void* __cdecl CoreMemory_AllocZeroedTagged(int a1, int a2, uint16_t a3)
{
    const uint32_t totalBytes = static_cast<uint32_t>(a2) * static_cast<uint32_t>(a1);
    void* block = Mem_AllocTagged(totalBytes, a3);
    if (block != nullptr) {
        std::memset(block, 0, totalBytes);
    }
    return block;
}

HOOK(0x00691260, CoreMemory_AllocZeroedTagged, "auto");
