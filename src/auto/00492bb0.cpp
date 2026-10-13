#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static int __fastcall SceneLoader_SetSecondarySceneIndex(void* self, void* /*edx*/, int sceneIndex)
{
    CSF_AT(int, self, 0x1F8) = sceneIndex;
    return sceneIndex;
}

HOOK(0x492BB0, SceneLoader_SetSecondarySceneIndex, "auto");
