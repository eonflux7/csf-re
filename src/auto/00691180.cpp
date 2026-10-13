#include "globals.h"
#include "hook.h"

// Original tail-jumps to FreeBlock_690F70 (cdecl, one argument, caller cleans up).
CSF_FUNC(int, __cdecl, FreeBlock_690F70, 0x00690F70, (void* block));

static int __cdecl CoreMemory_FreeBlockForward(void* block) {
    return FreeBlock_690F70(block);
}

HOOK(0x00691180, CoreMemory_FreeBlockForward, "auto");
