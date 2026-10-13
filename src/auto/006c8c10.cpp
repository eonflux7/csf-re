#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, DestroyBlockPayload, 0x006C8B90, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_GLOBAL(int32_t, g_Value_8BB4CC, 0x008BB4CC);

// Releases the global memory block if one is held: tears down its payload,
// frees the block through the game heap, and clears the global pointer.
static int __cdecl ReleaseSharedMemoryBlock()
{
    int result = 0;
    if (g_Value_8BB4CC != 0)
    {
        void* block = reinterpret_cast<void*>(static_cast<uintptr_t>(g_Value_8BB4CC));
        DestroyBlockPayload(block, nullptr);
        result = Mem_Free(block);
        g_Value_8BB4CC = 0;
    }
    return result;
}

HOOK(0x6C8C10, ReleaseSharedMemoryBlock, "auto");
