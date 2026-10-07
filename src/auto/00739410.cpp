#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Two global heap blocks freed at shutdown (or on reset).
CSF_GLOBAL(void*, g_Block8BBF90, 0x8BBF90);
CSF_GLOBAL(void*, g_Block8BBF8C, 0x8BBF8C);

// Heap free routine used for both blocks.
CSF_FUNC(int, __cdecl, FreeBlock_690F70, 0x690F70, (void* block));

static void* __cdecl FreeGlobalBlocks()
{
    if ( g_Block8BBF90 )
        FreeBlock_690F70(g_Block8BBF90);

    void* block = g_Block8BBF8C;
    if ( block )
        return reinterpret_cast<void*>(static_cast<uintptr_t>(FreeBlock_690F70(block)));
    return block;
}

HOOK(0x739410, FreeGlobalBlocks, "auto");
