#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, SceneLoader_UpdateCurrentScene, 0x00453CB0, (void* self, void* edx, char a2));
CSF_FUNC(int, __fastcall, SceneLoader_CleanupCurrentScene, 0x0044FB50, (void* self, void* edx));

// Tears down the current scene: updates it with flag 0, runs cleanup, then
// releases the object held at +0x188 through its virtual slot 0 (deleting dtor, flag 1).
static int __fastcall CScene_TearDown(C_008312b0* self, void* /*edx*/) {
    int result;

    SceneLoader_UpdateCurrentScene(self, nullptr, 0);
    result = SceneLoader_CleanupCurrentScene(self, nullptr);

    void* subsystem = CSF_AT(void*, self, 0x188);
    if (subsystem) {
        using ReleaseFn = int(__fastcall*)(void* self, void* edx, int deleteFlag);
        ReleaseFn release = reinterpret_cast<ReleaseFn>(CSF_AT(void**, subsystem, 0)[0]);
        result = release(subsystem, nullptr, 1);
        CSF_AT(void*, self, 0x188) = nullptr;
    }
    return result;
}

HOOK(0x004549B0, CScene_TearDown, "auto");
