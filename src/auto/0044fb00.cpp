#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(int, g_CurrentSceneIndex, 0x0089972C);
CSF_GLOBAL(void**, g_SceneEntries, 0x00899730);
CSF_GLOBAL(unsigned int, g_numScenes, 0x00899738);
CSF_GLOBAL(void*, g_AuxiliaryScene, 0x008AFC88);
CSF_GLOBAL(uint8_t, g_SceneLoaderMainScenePresent, 0x008AFC8C);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// Reports whether the current mission's scene prerequisites are satisfied:
// either the current scene index is valid, a current scene exists and its
// scene entry is populated, or the main scene loader is present with an
// auxiliary scene loaded.
static int MissionInfo_IsPrerequisiteMet()
{
    int sceneIndex = g_CurrentSceneIndex;
    if (sceneIndex >= 0
        && static_cast<unsigned int>(sceneIndex) < g_numScenes
        && g_pCurrentScene != nullptr
        && g_SceneEntries[sceneIndex] != nullptr)
    {
        return 1;
    }

    if (g_SceneLoaderMainScenePresent != 0 && g_AuxiliaryScene != nullptr)
    {
        return 1;
    }

    return 0;
}

HOOK(0x0044FB00, MissionInfo_IsPrerequisiteMet, "auto");
