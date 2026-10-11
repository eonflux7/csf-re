#include <cstdint>

#include "globals.h"
#include "hook.h"

// Callees, bound by address with the signatures given in the packet.
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());
CSF_FUNC(int, __fastcall, SceneLoader_GetResourceById, 0x007567C0, (void* self, void* /*edx*/, int index));
CSF_FUNC(int, __fastcall, sub_50EC40, 0x0050EC40, (void* self, void* /*edx*/, int a4, int a5));

// Refreshes the multiplayer pause menu's global state using the scene loader
// resources at index 0 and index 1. Both branches follow the same pattern:
// fetch the global value, take its sub-object at +0x22A4 as the loader, look up
// the resource, then pass it to sub_50EC40 together with the slot number.
static int __fastcall MenuMpPause_RefreshGlobalState(void* self, void* /*edx*/)
{
    // First slot (0)
    int globalValue = GetGlobalValue_8BC1E0();
    void* sceneLoader = reinterpret_cast<void*>(static_cast<uintptr_t>(globalValue) + 0x22a4);
    int resourceById = SceneLoader_GetResourceById(sceneLoader, nullptr, 0);
    sub_50EC40(self, nullptr, resourceById, 0);

    // Second slot (1)
    globalValue = GetGlobalValue_8BC1E0();
    sceneLoader = reinterpret_cast<void*>(static_cast<uintptr_t>(globalValue) + 0x22a4);
    int resourceById1 = SceneLoader_GetResourceById(sceneLoader, nullptr, 1);
    return sub_50EC40(self, nullptr, resourceById1, 1);
}

HOOK(0x0050EE10, MenuMpPause_RefreshGlobalState, "auto");
