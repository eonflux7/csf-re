#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(uint32_t*, __cdecl, NetSession_GetNodeAllocator, 0x00646FF0, ());
CSF_FUNC(char*, __cdecl, NetGatheringDdl_AllocateNode, 0x00647070,
         (int a1, int a2, int a3, int a4, int a5));
CSF_FUNC(void, __stdcall, NetGatheringDdl_DeallocateNode, 0x006A6D80,
         (int a2, int _C));
CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));

// std::list<>::_Buynode() for a NetZ-allocated list: allocates the 12-byte
// head node and constructs its next/prev links pointing at itself.
static char* __fastcall NetGatheringDdl_CreateListSentinel(
    void* /*self*/, void* /*edx*/)
{
    uint32_t* allocator = NetSession_GetNodeAllocator();
    char* node = nullptr;

    // the allocation is outside the guarded region: cleanup is armed only once it has returned
    node = NetGatheringDdl_AllocateNode(
        static_cast<int>(reinterpret_cast<uintptr_t>(allocator)),
        12,
        static_cast<int>(reinterpret_cast<uintptr_t>(
            CSF_STR(0x0085802C, "Unknown"))),
        0,
        7);

    try
    {
        // placement-construct _Next
        if (node)
            CSF_AT(char*, node, 0x0) = node;

        // placement-construct _Prev
        if (reinterpret_cast<uintptr_t>(node) + 4 != 0)
            CSF_AT(char*, node, 0x4) = node;
    }
    catch (...)
    {
        NetGatheringDdl_DeallocateNode(
            static_cast<int>(reinterpret_cast<uintptr_t>(node)), 1);
        RethrowCppException(nullptr, nullptr);
    }

    return node;
}

HOOK(0x006A6F10, NetGatheringDdl_CreateListSentinel, "auto");
