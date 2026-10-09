#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(uint32_t, __fastcall, CGuiControl_GetField012C, 0x0044F630, (CGuiControl* self, void*));
CSF_FUNC(int, __cdecl, SceneLoader_SelectScene, 0x00450630, (int a1, int a2));
CSF_FUNC(char, __fastcall, SceneLoader_ProcessSceneObject, 0x004DE7C0, (void* self, void* edx, void* a2));
CSF_FUNC(int, __fastcall, CSceneLoaderBase_ProcessCurrentScene, 0x00500800, (void* self, void* edx));

static int __fastcall CSceneLoaderBase_ProcessLoadedScenes(
    C_0083f578* self, void* /*edx*/, int /*unused*/)
{
    uint32_t sceneIndex = 0;
    uint32_t entryOffset = 0;

    for (;;)
    {
        const uint32_t entries = CSF_AT(uint32_t, self, 0x30);
        if (!entries)
            break;

        const int32_t byteCount =
            static_cast<int32_t>(CSF_AT(uint32_t, self, 0x34) - entries);
        if (sceneIndex >= static_cast<uint32_t>(byteCount / 88))
            break;

        auto* entry = reinterpret_cast<uint8_t*>(
            CSF_AT(uint32_t, self, 0x30)) + entryOffset;

        if (CSF_AT(uint8_t, entry, 0x1D))
        {
            SceneLoader_SelectScene(static_cast<int>(sceneIndex), 0);

            auto* scene = static_cast<CGuiControl*>(g_pCurrentScene);
            uint32_t objectIndex = 0;
            if (scene->m_id.alloc)
            {
                do
                {
                    void* object =
                        reinterpret_cast<void**>(scene->field_0010)[objectIndex];
                    if (object)
                    {
                        const uint32_t context = CGuiControl_GetField012C(scene, nullptr);
                        SceneLoader_ProcessSceneObject(
                            reinterpret_cast<void*>(context), nullptr, object);
                        scene = static_cast<CGuiControl*>(g_pCurrentScene);
                    }

                    ++objectIndex;
                }
                while (objectIndex < static_cast<uint32_t>(scene->m_id.alloc));
            }

            if (CSF_AT(uint32_t, self, 0x0C) == sceneIndex)
                CSceneLoaderBase_ProcessCurrentScene(self, nullptr);
        }

        ++sceneIndex;
        entryOffset += 88;
    }

    return SceneLoader_SelectScene(CSF_AT(int, self, 0x0C), 0);
}

HOOK(0x005008D0, CSceneLoaderBase_ProcessLoadedScenes, "auto");
