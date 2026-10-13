#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Original free routine for game-heap blocks (FreeBlock_690F70 @ 0x690F70, __cdecl, one argument).
CSF_FUNC(int, __cdecl, FreeBlock_690F70, 0x00690F70, (void* block));

// Frees the block stored at offset +0x58 of the context object and returns the free routine's result.
static int __cdecl RtFS_FreeStoredBlock(int a1)
{
    void* storedBlock = CSF_AT(void*, reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(a1)), 0x58);
    return FreeBlock_690F70(storedBlock);
}

HOOK(0x0069FB80, RtFS_FreeStoredBlock, "auto");
