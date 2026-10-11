#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __cdecl, NetSession_GetNodeAllocator, 0x00646FF0, ());
CSF_FUNC(void, __cdecl, NetSession_FreeListNode, 0x006470E0, (int a1, int a2, int a3));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Walks the circular node list headed at field_0054, unlinks every real node,
// returns it to the NetSession node free list, and frees its payload.
static int __fastcall C_0085fe28_ReleaseAllListNodes(C_0085fe28* self, void* /*edx*/)
{
    int result;
    for (result = self->field_0058; result; result = self->field_0058)
    {
        uint8_t* listHead = reinterpret_cast<uint8_t*>(self->field_0054);
        uint8_t* node = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, listHead, 0));
        uint32_t payload = CSF_AT(uint32_t, node, 0x8);

        if (node != listHead)
        {
            uint8_t* prevNode = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, node, 0x4));
            uint8_t* nextNode = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, node, 0x0));
            CSF_AT(uint32_t, prevNode, 0x0) = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(nextNode));
            CSF_AT(uint32_t, nextNode, 0x4) = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(prevNode));

            int allocator = static_cast<int>(reinterpret_cast<uintptr_t>(NetSession_GetNodeAllocator()));
            NetSession_FreeListNode(allocator, static_cast<int>(reinterpret_cast<uintptr_t>(node)), 7);
            --self->field_0058;
        }

        if (payload)
            Mem_Free(reinterpret_cast<void*>(static_cast<uintptr_t>(payload)));
    }
    return result;
}

HOOK(0x006A9A60, C_0085fe28_ReleaseAllListNodes, "auto");
