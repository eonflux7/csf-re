#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(uint32_t*, __cdecl, NetSession_GetNodeAllocator, 0x00646FF0, ());
CSF_FUNC(char*, __cdecl, NetSession_AllocateNode, 0x00647070,
         (int a1, int a2, int a3, int a4, int a5));
CSF_FUNC(void, __stdcall, NetGatheringDdl_DeallocateListNode, 0x006A6D80,
         (int a2, int _C));
CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));

// std::list<T, NetZ allocator>::_Buynode(): allocates a 20-byte node
// (next, prev, 12-byte value) and links it to itself.
static char* __fastcall NetGatheringDdl_BuyListHeadNode(
    void* /*self*/, void* /*edx*/)
{
    char* node = nullptr;

    // the allocation is outside the guarded region: cleanup is armed only once it has returned
    uint32_t* allocator = NetSession_GetNodeAllocator();
    node = NetSession_AllocateNode(
        reinterpret_cast<int>(allocator),
        20,
        reinterpret_cast<int>(CSF_STR(0x0085802C, "Unknown")),
        0,
        7);
    
    try
    {
        // construct(&node->next, node)
        if (node)
            CSF_AT(char*, node, 0x0) = node;

        // construct(&node->prev, node)
        if (reinterpret_cast<uintptr_t>(node) != static_cast<uintptr_t>(-4))
            CSF_AT(char*, node, 0x4) = node;
    }
    catch (...)
    {
        NetGatheringDdl_DeallocateListNode(reinterpret_cast<int>(node), 1);
        RethrowCppException(nullptr, nullptr);
    }

    return node;
}

HOOK(0x006A6E70, NetGatheringDdl_BuyListHeadNode, "auto");
