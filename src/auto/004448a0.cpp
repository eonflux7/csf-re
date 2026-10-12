#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, SceneLoader_ResetSceneRegions, 0x00469070, (S_004686a0* self, void*));

// Original: mov ecx, [ecx+4]; jmp SceneLoader_ResetSceneRegions
// Plain tail call, no stack arguments, so the callee pops nothing (ret 0).
static void __fastcall SceneLoader_ResetSceneObjects(void* self, void* /*edx*/) {
    S_004686a0* sceneRegions = reinterpret_cast<S_004686a0*>(CSF_AT(uint32_t, self, 0x4));
    SceneLoader_ResetSceneRegions(sceneRegions, nullptr);
}

HOOK(0x004448A0, SceneLoader_ResetSceneObjects, "auto");
