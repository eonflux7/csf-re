#include <cstddef>
#include <cstdint>

#include "globals.h"
#include "hook.h"

CSF_FUNC(std::size_t, __cdecl, SceneLoader_BeginMemoryMaintenance, 0x00691090, ());
CSF_FUNC(int32_t, __cdecl, GetGlobalValue_51F740, 0x0051F740, ());
CSF_FUNC(void, __fastcall, InvokeMember90VirtualSlot5, 0x0051F7A0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __cdecl, sub_6B16A0, 0x006B16A0, ());
CSF_FUNC(int, __cdecl, RwFreeListPurgeAllFreeLists, 0x006265F0, ());

static int __cdecl FlushSceneMemoryAndPurgeFreeLists()
{
    // Result of the first call is discarded by the original.
    SceneLoader_BeginMemoryMaintenance();

    if (GetGlobalValue_51F740() != 0)
    {
        // The original calls GetGlobalValue_51F740 a second time and passes
        // the result as `this` to the forwarder.
        int32_t globalValue = GetGlobalValue_51F740();
        InvokeMember90VirtualSlot5(reinterpret_cast<void*>(static_cast<intptr_t>(globalValue)), nullptr);
    }

    int globalInt = GetGlobalInt();
    sub_6B16A0();

    RwFreeListPurgeAllFreeLists();

    // Tail jump to SceneLoader_BeginMemoryMaintenance: its result is our result.
    return static_cast<int>(SceneLoader_BeginMemoryMaintenance());
}

HOOK(0x0050E500, FlushSceneMemoryAndPurgeFreeLists, "auto");
