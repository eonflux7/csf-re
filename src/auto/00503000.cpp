#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int32_t, __cdecl, GetGlobal8afc78, 0x0042D6D0, ());
CSF_FUNC(int, __fastcall, Hud_RefreshWidgetTextures, 0x00433BE0, (void* self, void*));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(void*, __stdcall, SceneLoader_RegisterLoadedScene, 0x004FDF50, (int a1));
CSF_FUNC(char, __fastcall, SceneLoader_PrepareScene, 0x004FE010, (void* self, void* edx, char* a2));
CSF_FUNC(char, __fastcall, SceneLoader_PrepareSceneResources, 0x004FE990, (void* self, void* edx, char* a2, int a3));
CSF_FUNC(char, __fastcall, SceneLoader_PrepareNewScene, 0x004FEA80, (void* self, void* edx, char* a2, char a3));
CSF_FUNC(int, __fastcall, SceneLoader_FinalizeSceneLoad, 0x005021A0, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, CMenuMissionInfo_SetTextIfNonEmpty, 0x0053D4B0, (void* self, void*, char* source));
CSF_FUNC(int, __fastcall, SceneLoader_GetSceneController, 0x00652010, (void* self, void* edx));
CSF_FUNC(void, __cdecl, SetFallbackContext, 0x006912F0, ());
CSF_FUNC(char, __cdecl, Path_ReplaceExtension, 0x006947D0, (char* Str, int a2, char* a3));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, C_006cabc0_GetField0098, 0x006CABC0, (void* self, void*));
CSF_FUNC(int, __fastcall, SceneLoader_GetLoadedScene, 0x006CBAC0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, SceneLoader_ResetSceneController, 0x006CD0E0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(const char*, g_pszMenu_P_Tactic, 0x00899B14);

static char __fastcall CSceneLoader_LoadScene(
    C_0082d410* self, void* /*edx*/, char* sceneName, int reload, int /*unused*/)
{
    if (!CSF_AT(uint8_t, reinterpret_cast<void*>(GetGlobalInt()), 0x4))
    {
        Hud_RefreshWidgetTextures(
            reinterpret_cast<void*>(GetGlobal8afc78()), nullptr);
    }

    SceneLoader_PrepareScene(self, nullptr, sceneName);
    SceneLoader_PrepareSceneResources(self, nullptr, sceneName, 1);
    if (!static_cast<uint8_t>(reload))
        SceneLoader_PrepareNewScene(self, nullptr, sceneName, 1);

    GetValue8B0554();
    if (CSF_AT(uint8_t, self, 0xB6) &&
        CSF_AT(void*, reinterpret_cast<void*>(GetGlobalState_50D190()), 0x1C))
    {
        void* effect = CSF_AT(
            void*, reinterpret_cast<void*>(GetGlobalState_50D190()), 0x1C);
        reinterpret_cast<void(__fastcall*)(void*, void*, float)>(
            CSF_AT(void**, effect, 0)[0x44 / 4])(effect, nullptr, 0.0f);
    }

    void* sceneSource = reinterpret_cast<void*>(GetValue8B0554());
    char result = reinterpret_cast<char(__fastcall*)(void*, void*, char*, int)>(
        CSF_AT(void**, sceneSource, 0)[0x4 / 4])(
            sceneSource, nullptr, sceneName, reload);
    if (!result)
        return result;

    if (CSF_AT(uint8_t, self, 0xB6) &&
        CSF_AT(void*, reinterpret_cast<void*>(GetGlobalState_50D190()), 0x1C))
    {
        void* effect = CSF_AT(
            void*, reinterpret_cast<void*>(GetGlobalState_50D190()), 0x1C);
        reinterpret_cast<void(__fastcall*)(void*, void*, float)>(
            CSF_AT(void**, effect, 0)[0x44 / 4])(effect, nullptr, 10.0f);
    }

    auto* sceneData = reinterpret_cast<uint8_t*>(GetValue8B0554()) + 84;
    if (CSF_AT(uint8_t, reinterpret_cast<void*>(GetGlobalInt()), 0x4))
    {
        bool enabled = CSF_AT(uint8_t, sceneData, 7896) ||
                       CSF_AT(uint8_t, sceneData, 7897) ||
                       CSF_AT(uint8_t, sceneData, 7904) ||
                       CSF_AT(uint8_t, sceneData, 7898);
        CSF_AT(uint8_t, reinterpret_cast<void*>(GetGlobalState_50D190()), 208) =
            enabled;
    }

    void* menu = CSF_AT(
        void*, reinterpret_cast<void*>(GetGlobalState_50D190()), 20);
    void* missionInfo =
        reinterpret_cast<void*(__fastcall*)(void*, void*, const char*)>(
            CSF_AT(void**, menu, 0)[0x5C / 4])(
                menu, nullptr, g_pszMenu_P_Tactic);
    char* missionText = CSF_AT(uint32_t, sceneData, 4552) < 0x10u
        ? reinterpret_cast<char*>(sceneData + 4532)
        : CSF_AT(char*, sceneData, 4532);
    CMenuMissionInfo_SetTextIfNonEmpty(missionInfo, nullptr, missionText);

    auto* controller = reinterpret_cast<C_00860bb8*>(
        SceneLoader_GetSceneController(
            reinterpret_cast<void*>(GetGlobalInt()), nullptr));

    char modelPath[256];
    if (CSF_AT(uint32_t, reinterpret_cast<void*>(GetGlobalValue_8BC1E0()), 72))
    {
        auto* pathSource = reinterpret_cast<uint8_t*>(GetGlobalValue_8BC1E0());
        const char* path = CSF_AT(uint32_t, pathSource, 76) < 0x10
            ? reinterpret_cast<const char*>(pathSource + 52 + 4)
            : CSF_AT(const char*, pathSource, 52 + 4);
        std::strcpy(modelPath, path);
    }
    else
    {
        std::strcpy(modelPath, sceneName);
    }

    Path_ReplaceExtension(
        modelPath, 256, const_cast<char*>(CSF_STR(0x0083F56C, ".m3d")));

    struct LoadParameters
    {
        uint8_t specialMode;
        uint8_t padding0[3];
        char* primaryPath;
        char* secondaryPath;
        char* resourcePath;
        uint32_t resourceValue0;
        uint32_t resourceValue1;
        uint32_t resourceValue2;
        uint8_t resourceFlag;
        uint8_t padding1[3];
        char* auxiliaryPath;
        char* modelPath;
        uint8_t reserved[8];
    };
    static_assert(sizeof(LoadParameters) == 0x30);
    LoadParameters parameters{};
    parameters.resourceFlag = 1;
    parameters.specialMode =
        CSF_AT(uint8_t, sceneData, 7896) ||
        CSF_AT(uint8_t, sceneData, 7897) ||
        CSF_AT(uint8_t, sceneData, 7898) ||
        CSF_AT(uint8_t, sceneData, 7904);

    auto* primarySource = reinterpret_cast<uint8_t*>(GetValue8B0554());
    parameters.primaryPath = CSF_AT(uint32_t, primarySource, 8044) < 0x10
        ? reinterpret_cast<char*>(primarySource + 8020 + 4)
        : CSF_AT(char*, primarySource, 8020 + 4);

    parameters.resourcePath = CSF_AT(uint32_t, sceneData, 1100) < 0x10u
        ? reinterpret_cast<char*>(sceneData + 1080)
        : CSF_AT(char*, sceneData, 1080);
    parameters.resourceValue0 = CSF_AT(uint32_t, sceneData, 1108);
    parameters.resourceValue1 = CSF_AT(uint32_t, sceneData, 1112);
    parameters.resourceValue2 = CSF_AT(uint32_t, sceneData, 1116);
    parameters.resourceFlag = CSF_AT(uint8_t, sceneData, 1120);

    auto* auxiliarySource = reinterpret_cast<uint8_t*>(GetValue8B0554());
    parameters.auxiliaryPath = CSF_AT(uint32_t, auxiliarySource, 8072) < 0x10
        ? reinterpret_cast<char*>(auxiliarySource + 8048 + 4)
        : CSF_AT(char*, auxiliarySource, 8048 + 4);
    parameters.modelPath = modelPath;

    if (CSF_AT(uint32_t, sceneData, 2196))
    {
        parameters.secondaryPath = CSF_AT(uint32_t, sceneData, 2200) < 0x10u
            ? reinterpret_cast<char*>(sceneData + 2180)
            : CSF_AT(char*, sceneData, 2180);
    }
    else
    {
        auto* secondarySource = reinterpret_cast<uint8_t*>(GetValue8B0554());
        parameters.secondaryPath =
            CSF_AT(uint32_t, secondarySource, 8044) < 0x10
                ? reinterpret_cast<char*>(secondarySource + 8020 + 4)
                : CSF_AT(char*, secondarySource, 8020 + 4);
    }

    if (static_cast<uint8_t>(reload))
    {
        if (C_006cabc0_GetField0098(controller, nullptr))
        {
            SceneLoader_ResetSceneController(controller, nullptr);
            result = static_cast<char>(
                reinterpret_cast<int(__fastcall*)(void*, void*, char*)>(
                    controller->vtbl[0x20 / 4])(
                        controller, nullptr, parameters.modelPath));
        }
    }
    else
    {
        result = controller->vSlot24(reinterpret_cast<uint8_t*>(&parameters));
    }

    if (result)
    {
        void* source = reinterpret_cast<void*>(GetValue8B0554());
        uint32_t sceneValue =
            CSF_AT(uint32_t, CSF_AT(void*, source, 8120), 16);
        reinterpret_cast<void(__fastcall*)(void*, void*, uint32_t)>(
            controller->vtbl[0xB8 / 4])(controller, nullptr, sceneValue);
        int loadedScene = SceneLoader_GetLoadedScene(controller, nullptr);
        if (loadedScene)
            SceneLoader_RegisterLoadedScene(loadedScene);
    }

    SetFallbackContext();
    SceneLoader_FinalizeSceneLoad(
        self, nullptr, reinterpret_cast<int>(sceneName));

    if (CSF_AT(uint8_t, reinterpret_cast<void*>(GetGlobalInt()), 0x4))
    {
        Hud_RefreshWidgetTextures(
            reinterpret_cast<void*>(GetGlobal8afc78()), nullptr);
    }

    if (CSF_AT(uint8_t, self, 0xB6) &&
        CSF_AT(void*, reinterpret_cast<void*>(GetGlobalState_50D190()), 0x1C))
    {
        void* effect = CSF_AT(
            void*, reinterpret_cast<void*>(GetGlobalState_50D190()), 0x1C);
        reinterpret_cast<void(__fastcall*)(void*, void*, float)>(
            CSF_AT(void**, effect, 0)[0x44 / 4])(effect, nullptr, 10.0f);
    }

    return result;
}

HOOK(0x00503000, CSceneLoader_LoadScene, "auto");
