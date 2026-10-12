#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Resets a buffer object: clears its flag byte, releases the debug-allocated
// block at +0x20, zeroes the three pointer/size slots at +0x20..+0x28, then
// releases the game-heap block at +0x0C and clears it.
static int __fastcall ResetBufferAndReleaseBlocks(void* self, void* /*edx*/)
{
    if (CSF_AT(uint8_t, self, 0x00) != 0)
        CSF_AT(uint8_t, self, 0x00) = 0;

    void* debugBlock = CSF_AT(void*, self, 0x20);
    if (debugBlock != nullptr)
        Mem_FreeDebug(debugBlock);

    CSF_AT(uint32_t, self, 0x20) = 0;
    CSF_AT(uint32_t, self, 0x24) = 0;
    CSF_AT(uint32_t, self, 0x28) = 0;

    int result = 0;
    void* heapBlock = CSF_AT(void*, self, 0x0C);
    if (heapBlock != nullptr)
    {
        result = Mem_Free(heapBlock);
        CSF_AT(uint32_t, self, 0x0C) = 0;
    }
    return result;
}

HOOK(0x4E00F0, ResetBufferAndReleaseBlocks, "auto");
