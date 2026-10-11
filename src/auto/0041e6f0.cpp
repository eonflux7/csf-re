#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

CSF_GLOBAL(void*, g_SceneCachedBlock, 0x00899504);
CSF_GLOBAL(int, g_SceneCounterA, 0x00899508);
CSF_GLOBAL(int, g_SceneCounterB, 0x0089950C);
CSF_GLOBAL(float, g_SceneFloatDefault, 0x00899514);

// Clears the scene's global state: zeroes two counters, frees the cached block
// if one exists (and clears the pointer), and resets the float to -1.0f.
static int Scene_ResetGlobalObjects() {
    int result = reinterpret_cast<int>(g_SceneCachedBlock);
    g_SceneCounterA = 0;
    g_SceneCounterB = 0;
    if (result != 0) {
        // eax keeps Mem_Free's return value, as the original does.
        result = Mem_Free(g_SceneCachedBlock);
        g_SceneCachedBlock = nullptr;
    }
    g_SceneFloatDefault = -1.0f;
    return result;
}

HOOK(0x0041E6F0, Scene_ResetGlobalObjects, "auto");
