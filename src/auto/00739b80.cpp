#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
static std::uint8_t __fastcall DestroyTreeBranch(void* tree, void* edx, void** block);  // self-recursion: call the reimplementation directly

// __thiscall: the tree in ecx is only passed on to the recursion (as the original does).
static std::uint8_t __fastcall DestroyTreeBranch(void* tree, void* /*edx*/, void** block)
{
    void** node = block;
    void** blockToFree = block;
    std::uint8_t sentinelFlag = CSF_AT(std::uint8_t, node, 0x49);

    while (sentinelFlag == 0)
    {
        DestroyTreeBranch(tree, nullptr, CSF_AT(void**, node, 0x08));
        node = CSF_AT(void**, node, 0x00);

        if (blockToFree != nullptr)
            Mem_FreeDebug(blockToFree);

        sentinelFlag = CSF_AT(std::uint8_t, node, 0x49);
        blockToFree = node;
    }

    return sentinelFlag;
}

HOOK(0x00739B80, DestroyTreeBranch, "auto");
