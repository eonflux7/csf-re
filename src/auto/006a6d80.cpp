#include "hook.h"
#include "globals.h"

CSF_FUNC(uint32_t*, __cdecl, NetSession_GetNodeAllocator, 0x00646FF0, ());
CSF_FUNC(void, __cdecl, NetSession_FreeListNode, 0x006470E0, (int a1, int a2, int a3));

static void __stdcall GatheringDdl_ReleaseSessionNode(int nodeHandle, int /*unused*/)
{
    int nodeAllocator = (int)NetSession_GetNodeAllocator();
    NetSession_FreeListNode(nodeAllocator, nodeHandle, 7);
}

HOOK(0x006A6D80, GatheringDdl_ReleaseSessionNode, "auto");
