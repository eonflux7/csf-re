#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(char, __cdecl, MemDebug_FreeDebugBlock, 0x00690E60, (void* block));

static char __fastcall DestroyEntryTreeBranch(void* self, void* /*edx*/, void** block)
{
    // The child at +8 is recursive; the left link at +0 is loaded before freeing.
    // ECX is forwarded unchanged, with one stack argument (ret 4).
    char sentinel = CSF_AT(char, block, 0x15);
    while (!sentinel)
    {
        DestroyEntryTreeBranch(self, nullptr, CSF_AT(void**, block, 8));
        void** next = CSF_AT(void**, block, 0);
        if (block)
            MemDebug_FreeDebugBlock(block);
        block = next;
        sentinel = CSF_AT(char, block, 0x15);
    }
    return sentinel;
}

HOOK(0x0069A860, DestroyEntryTreeBranch, "auto");
