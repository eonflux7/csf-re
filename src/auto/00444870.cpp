#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Tail-jumps into SceneLoader_PrepareSceneActors with this = this[1].
CSF_FUNC(int, __fastcall, SceneLoader_PrepareSceneActors, 0x0046B170, (S_0044d360* self, void*));

static int __fastcall SceneLoader_PrepareEnvironment(S_0044d360** self, void* /*edx*/)
{
    return SceneLoader_PrepareSceneActors(self[1], nullptr);
}

HOOK(0x444870, SceneLoader_PrepareEnvironment, "auto");
