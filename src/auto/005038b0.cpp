#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_401090, 0x00401090, ());
CSF_FUNC(int, __fastcall, SceneLoader_ResetGlobalService, 0x00401970, (void* self, void* edx));
CSF_FUNC(int32_t, __cdecl, GetGlobal8afc78, 0x0042D6D0, ());
CSF_FUNC(int, __fastcall, HudWidget_ForwardUpdateToSubobject, 0x0042DFB0, (void* self, void*));
CSF_FUNC(int, __cdecl, GetValue_8afc80, 0x004419C0, ());
CSF_FUNC(int, __fastcall, SceneLoader_ResetSceneService, 0x00442130, (void* self, void* edx));
CSF_FUNC(char, __fastcall, SceneLoader_ClearSecondaryResources, 0x00445240, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_ClearAdditionalResources, 0x0044C540, (void* self, void* edx));
CSF_FUNC(int, __cdecl, SceneLoader_SelectScene, 0x00450630, (int a1, int a2));
CSF_FUNC(char, __fastcall, SceneLoader_SetCurrentSceneState, 0x00453CB0, (void* self, void* edx, char a2));
CSF_FUNC(unsigned int, __fastcall, SceneLoader_ClearPrimaryResources, 0x00456D70, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobal8B0558, 0x004F4920, ());
CSF_FUNC(uint32_t*, __fastcall, SceneLoader_ResetSharedResources, 0x004F4950, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(int, __fastcall, SceneLoader_ClearGlobalSceneResources, 0x004FC550, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_FinalizeSceneCleanup, 0x004FE520, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_ClearSceneEntries, 0x00503710, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int32_t, __cdecl, GetGlobalValue_51F740, 0x0051F740, ());
CSF_FUNC(int, __fastcall, SceneLoader_SetServiceState, 0x00520420, (void* self, void* edx, char a2));
CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(void, __fastcall, ApplyGlobalValueSettings, 0x005233D0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Get_008b0678, 0x00525130, ());
CSF_FUNC(int, __fastcall, SceneLoader_ResetSettingsService, 0x00525970, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_GetResourceInterface, 0x00652010, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(unsigned int, __fastcall, SceneLoader_ReleaseEntryResource, 0x006B1840, (void* self, void* edx, int a2));

CSF_GLOBAL(unsigned int, g_numScenes, 0x00899738);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __fastcall CSceneLoaderBase_UnloadScene(
    CSceneLoaderBase* self, void* /*edx*/, char preserveSceneEntries)
{
    if (!preserveSceneEntries)
    {
        SceneLoader_ClearGlobalSceneResources(
            reinterpret_cast<void*>(GetGlobalValue_4FBDE0()), nullptr);
        SceneLoader_ClearPrimaryResources(CSF_AT(void*, self, 0xE4), nullptr);
        SceneLoader_ClearSecondaryResources(CSF_AT(void*, self, 0xE8), nullptr);
        SceneLoader_ClearAdditionalResources(CSF_AT(void*, self, 0xEC), nullptr);
    }

    SceneLoader_ResetSharedResources(
        reinterpret_cast<void*>(GetGlobal8B0558()), nullptr);
    HudWidget_ForwardUpdateToSubobject(
        reinterpret_cast<void*>(GetGlobal8afc78()), nullptr);

    if (!preserveSceneEntries)
    {
        const unsigned int sceneCount = g_numScenes;
        for (unsigned int i = 0; i < sceneCount; ++i)
        {
            SceneLoader_SelectScene(static_cast<int>(i), 0);
            SceneLoader_SetCurrentSceneState(g_pCurrentScene, nullptr, 0);
        }

        const uint32_t currentScene = CSF_AT(uint32_t, self, 0x0C);
        if (currentScene != static_cast<uint32_t>(-1))
            SceneLoader_SelectScene(static_cast<int>(currentScene), 0);

        const uint32_t initialEntryCount = CSF_AT(uint32_t, self, 0x44);
        *reinterpret_cast<uint8_t*>(&self->field_0008) = 0;

        if (initialEntryCount)
        {
            uint32_t entryOffset = 0;
            uint32_t entryIndex = 0;
            do
            {
                auto* entry = CSF_AT(uint8_t*, self, 0x3C) + entryOffset;
                const int resource = CSF_AT(int, entry, 0x40);
                if (resource)
                {
                    SceneLoader_ReleaseEntryResource(
                        reinterpret_cast<void*>(GetGlobalInt()), nullptr, resource);
                    CSF_AT(int, entry, 0x40) = 0;
                }
                const uint32_t entryCount = CSF_AT(uint32_t, self, 0x44);
                ++entryIndex;
                entryOffset += 68;
                if (entryIndex >= entryCount)
                    break;
            } while (true);
        }

        SceneLoader_ClearSceneEntries(self, nullptr);
    }

    SceneLoader_FinalizeSceneCleanup(self, nullptr);
    SceneLoader_ResetSceneService(
        reinterpret_cast<void*>(GetValue_8afc80()), nullptr);
    SceneLoader_SetServiceState(
        reinterpret_cast<void*>(GetGlobalValue_51F740()), nullptr, 1);
    SceneLoader_ResetGlobalService(
        reinterpret_cast<void*>(GetGlobalValue_401090()), nullptr);
    SceneLoader_ResetSettingsService(
        reinterpret_cast<void*>(Get_008b0678()), nullptr);
    ApplyGlobalValueSettings(
        reinterpret_cast<void*>(GetGlobalValue_521BB0()), nullptr);

    if (CSF_AT(uint32_t, self, 0x74))
    {
        void* resourceInterface = reinterpret_cast<void*>(
            SceneLoader_GetResourceInterface(
                reinterpret_cast<void*>(GetGlobalInt()), nullptr));
        reinterpret_cast<void (__fastcall*)(void*, void*)>(
            CSF_AT(void**, resourceInterface, 0)[0x30 / 4])(
                resourceInterface, nullptr);
    }

    void* state = reinterpret_cast<void*>(GetGlobalState_50D190());
    void* sceneInterface = CSF_AT(void*, state, 0x14);
    return reinterpret_cast<int (__fastcall*)(void*, void*)>(
        CSF_AT(void**, sceneInterface, 0)[0x50 / 4])(
            sceneInterface, nullptr);
}

HOOK(0x005038B0, CSceneLoaderBase_UnloadScene, "auto");
