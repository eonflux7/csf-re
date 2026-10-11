#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static std::uint8_t __stdcall DestroyBlockTree(void** block)
{
    void** node = block;
    void** blockToFree = block;
    std::uint8_t sentinelFlag = CSF_AT(std::uint8_t, node, 0x11);

    while (sentinelFlag == 0)
    {
        DestroyBlockTree(CSF_AT(void**, node, 0x08));
        node = CSF_AT(void**, node, 0x00);

        if (blockToFree != nullptr)
            Mem_FreeDebug(blockToFree);

        sentinelFlag = CSF_AT(std::uint8_t, node, 0x11);
        blockToFree = node;
    }

    return sentinelFlag;
}

HOOK(0x00733D80, DestroyBlockTree, "auto");
