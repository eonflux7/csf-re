#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(int, g_CurrentSceneIndex, 0x0089972C);
CSF_GLOBAL(void**, g_SceneTableBase, 0x00899730);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_GLOBAL(uint8_t, g_SceneLoaderMainScenePresent, 0x008AFC8C);

static int SceneLoader_SelectScene(int sceneIndex, int /*unused2*/) {
    g_CurrentSceneIndex = sceneIndex;
    g_pCurrentScene = g_SceneTableBase[sceneIndex];
    g_SceneLoaderMainScenePresent = 0;
    return sceneIndex;
}

HOOK(0x450630, SceneLoader_SelectScene, "auto");
