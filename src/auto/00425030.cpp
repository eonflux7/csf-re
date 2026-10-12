#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __stdcall, SceneList_AppendIdIfFound, 0x0041ECE0, (unsigned int a2));

static void __fastcall C_0082fcd8_ReleaseSceneListId(C_0082fcd8* self, void* /*edx*/, int /*a2*/) {
    uint32_t& sceneListId = CSF_AT(uint32_t, self, 0x44);
    if (sceneListId) {
        SceneList_AppendIdIfFound(sceneListId);
        CSF_AT(uint32_t, self, 0x44) = 0;
    }
}

HOOK(0x425030, C_0082fcd8_ReleaseSceneListId, "auto");
