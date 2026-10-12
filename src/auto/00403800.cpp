#include <cstddef>
#include "globals.h"
#include <cstdint>
#include "hook.h"

// 0x006910F0: __cdecl game heap allocator (caller pops its argument).
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));

// 0x00403800: __stdcall wrapper (ret 4). Forwards its single size argument to
// the game heap allocator and returns the resulting pointer as the raw 32-bit value.
static int __stdcall GameHeap_AllocateBlock(int size)
{
    void* block = Mem_Alloc(static_cast<std::size_t>(size));
    return static_cast<int>(reinterpret_cast<std::uintptr_t>(block));
}

HOOK(0x403800, GameHeap_AllocateBlock, "auto");
