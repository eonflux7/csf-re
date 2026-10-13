#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static int __fastcall SceneLoader_SetPrimarySceneIndex(void* self, void* /*edx*/, int sceneIndex)
{
    CSF_AT(int, self, 0x1F4) = sceneIndex;
    return sceneIndex;
}

HOOK(0x492BA0, SceneLoader_SetPrimarySceneIndex, "auto");
