#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

CSF_GLOBAL(int, g_CurrentSceneIndex, 0x0089972C);
CSF_GLOBAL(void**, g_SceneEntries, 0x00899730);
CSF_GLOBAL(uint32_t, g_SceneEntryCapacity, 0x00899734);
CSF_GLOBAL(unsigned int, g_numScenes, 0x00899738);
CSF_GLOBAL(void*, g_MainScene, 0x008AFC88);
CSF_GLOBAL(uint8_t, g_SceneLoaderMainScenePresent, 0x008AFC8C);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __cdecl SceneLoader_ResetEntries()
{
    void** entries = g_SceneEntries;
    g_SceneLoaderMainScenePresent = 0;

    for (uint32_t index = 0; index < g_numScenes; ++index)
    {
        g_CurrentSceneIndex = static_cast<int>(index);
        g_pCurrentScene = entries[index];
        g_SceneLoaderMainScenePresent = 0;

        void* scene = entries[index];
        if (scene)
        {
            void* sceneBase = static_cast<uint8_t*>(scene) + 4;
            ((int(__fastcall*)(void*, void*, int))
                CSF_AT(void**, sceneBase, 0)[0])(sceneBase, nullptr, 1);

            g_SceneEntries[index] = nullptr;
            entries = g_SceneEntries;
        }
    }

    g_SceneEntryCapacity = 0;
    g_numScenes = 0;
    if (entries)
    {
        Mem_Free(entries);
        g_SceneEntries = nullptr;
    }

    void* mainScene = g_MainScene;
    int result = 0;
    if (mainScene)
    {
        void* sceneBase = static_cast<uint8_t*>(mainScene) + 4;
        g_pCurrentScene = mainScene;
        g_SceneLoaderMainScenePresent = 1;

        result = ((int(__fastcall*)(void*, void*, int))
            CSF_AT(void**, sceneBase, 0)[0])(sceneBase, nullptr, 1);

        g_MainScene = nullptr;
        g_SceneLoaderMainScenePresent = 0;
    }

    g_pCurrentScene = nullptr;
    g_CurrentSceneIndex = -1;
    return result;
}

HOOK(0x004506B0, SceneLoader_ResetEntries, "auto");
