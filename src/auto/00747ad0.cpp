#include <cstdint>
#include "globals.h"
#include "hook.h"

// Original: 0x007512A0, __thiscall-style (ecx = this), returns void per the bind line.
CSF_FUNC(void, __fastcall, SceneLoader_ClearWorldResources, 0x007512A0, (void* self, void* edx));

// Original 0x00747AD0: stdcall(5 args), forwards arg2 as `this` to SceneLoader_ClearWorldResources.
static int __stdcall ProyectilDDL_ClearSceneResourcesFromArg2(int a1, int a2, int a3, int a4, int a5) {
    (void)a1;
    (void)a3;
    (void)a4;
    (void)a5;
    void* sceneLoader = reinterpret_cast<void*>(static_cast<uintptr_t>(static_cast<uint32_t>(a2)));
    SceneLoader_ClearWorldResources(sceneLoader, nullptr);
    return 0;
}

HOOK(0x00747AD0, ProyectilDDL_ClearSceneResourcesFromArg2, "auto");
