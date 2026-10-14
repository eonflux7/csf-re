#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(char, __cdecl, MemDebug_FreeDebugBlock, 0x00690E60, (void* block));

static char __fastcall DestroyNodeBranch(void* self, void* /*edx*/, void** block)
{
    // Same traversal as the original: recurse right, save left, then free.
    char sentinel = CSF_AT(char, block, 0x15);
    while (!sentinel)
    {
        DestroyNodeBranch(self, nullptr, CSF_AT(void**, block, 8));
        void** next = CSF_AT(void**, block, 0);
        if (block)
            MemDebug_FreeDebugBlock(block);
        block = next;
        sentinel = CSF_AT(char, block, 0x15);
    }
    return sentinel;
}

HOOK(0x006B6FE0, DestroyNodeBranch, "auto");
