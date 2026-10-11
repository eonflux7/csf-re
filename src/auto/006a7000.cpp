#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Bound by address. The disassembly pushes (node, 7) into 0x646FF0 and passes its
// return value to 0x6470E0, so the signatures below follow the real stack usage.
CSF_FUNC(uint32_t, __cdecl, NetSession_GetNodeAllocator, 0x00646FF0, (void* node, int kind));
CSF_FUNC(void, __cdecl, NetSession_FreeListNode, 0x006470E0, (uint32_t handle));

// Clears the intrusive node list owned by this object. Each node is detached,
// its owned sub-object is released via its vtable slot 0 (with arg 1), and the
// node is returned to the NetSession free list. The list count at +8 is reset to 0.
static uint32_t* __fastcall NetGatheringDdl_ClearNodeList(uint32_t* self, void* /*edx*/)
{
    uint32_t* head = CSF_AT(uint32_t*, self, 0x4);
    uint32_t* node = reinterpret_cast<uint32_t*>(head[0]);

    // Reset the sentinel to an empty list.
    head[0] = reinterpret_cast<uint32_t>(head);
    CSF_AT(uint32_t*, self, 0x4)[1] = reinterpret_cast<uint32_t>(CSF_AT(uint32_t*, self, 0x4));
    uint32_t* result = CSF_AT(uint32_t*, self, 0x4);
    CSF_AT(uint32_t, self, 0x8) = 0;

    if (node != CSF_AT(uint32_t*, self, 0x4)) {
        uint32_t* next = nullptr;
        do {
            // Read the successor before the node is released.
            next = reinterpret_cast<uint32_t*>(node[0]);

            // Node now carries the C_0085fe20 vtable.
            CSF_AT(void*, node, 0x8) = CSF_VTBL(0x0085FE20);

            // Release the owned sub-object (vtable slot 0, arg 1), if present.
            void* owner = reinterpret_cast<void*>(node[3]);
            if (owner != nullptr) {
                auto releaseFn = reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                    CSF_AT(void**, owner, 0)[0]);
                releaseFn(owner, nullptr, 1);
            }

            uint32_t handle = NetSession_GetNodeAllocator(node, 7);
            NetSession_FreeListNode(handle);

            result = CSF_AT(uint32_t*, self, 0x4);
            node = next;
        } while (next != result);
    }
    return result;
}

HOOK(0x6A7000, NetGatheringDdl_ClearNodeList, "auto");
