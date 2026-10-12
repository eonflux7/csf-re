#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, SceneLoader_PrepareSceneResources, 0x00469840, (S_0045d610* self, void*));

// Tail-jumps to SceneLoader_PrepareSceneResources with ecx = this[1]; edx is not used by the callee.
static int __fastcall SceneLoader_PrepareSceneObject(S_0045d610** self, void* /*edx*/)
{
    S_0045d610* loadedScene = self[1];
    return SceneLoader_PrepareSceneResources(loadedScene, nullptr);
}

HOOK(0x004448B0, SceneLoader_PrepareSceneObject, "auto");
