#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(CScene*, g_AuxiliaryScene, 0x008AFC88);
CSF_GLOBAL(uint8_t, g_SceneLoaderMainScenePresent, 0x008AFC8C);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// Promotes the auxiliary scene to the current scene when one is loaded.
// Returns the auxiliary scene pointer (null when none is set).
static CScene* __cdecl SceneLoader_ResetEntrySelection() {
    CScene* auxiliaryScene = g_AuxiliaryScene;
    if (auxiliaryScene) {
        g_pCurrentScene = auxiliaryScene;
        g_SceneLoaderMainScenePresent = 1;
    }
    return auxiliaryScene;
}

HOOK(0x0044F650, SceneLoader_ResetEntrySelection, "auto");
