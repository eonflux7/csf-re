#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobal8afc7c, 0x0043E010, ());
CSF_FUNC(int, __fastcall, Campaign_FindMission, 0x0043E4B0, (void* self, void* , char* sceneName));
CSF_FUNC(bool, __fastcall, Campaign_HasCurrentScene, 0x0043E610, (void* self, void*));
CSF_FUNC(bool, __fastcall, IsPendingIndexMatch_443230, 0x00443230, (void* self, void* edx, int pendingIndex));
CSF_FUNC(int, __fastcall, SceneLoader_ClearAdditionalResources, 0x0044C540, (void* self, void* edx));
CSF_FUNC(int, __fastcall, MissionScript_ResetPendingBlock, 0x00468830, (void* self, void* , char flag));
CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(void*, __fastcall, GetGlobalSubObject_4FC0A0, 0x004FC0A0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_8B0568, 0x004FDBB0, ());
CSF_FUNC(void*, __fastcall, UmsVirtualProcessor_GetDefaultDestination, 0x004FE4A0, (void* self, void*));
CSF_FUNC(std::uint32_t, __fastcall, SceneLoader_DestroyMenuBuffer, 0x004FF3C0, (std::uint32_t* self, void*));
CSF_FUNC(int, __fastcall, MenuBuffer_Init, 0x00501870, (uint8_t* self, void*));
CSF_FUNC(void, __fastcall, GlobalMenu_SetSelectedScenarioName, 0x00501990, (S_004ff0f0* self, void* , char* String1, char a3));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, Interface_SetMenuState, 0x0050DC10, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GameBddLoader_DispatchMenuRequest, 0x0050F790, (void* self, void* , int entry, int entryArgument, int requestData, int configuration));
CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(void, __fastcall, AudioSound_ForwardToGlobalObjectSlot29, 0x00521D90, (void* self, void* edx, int value));
CSF_FUNC(unsigned char, __fastcall, AudioSoundManager_FinishLoadContext, 0x005226A0, (void* self, void*));
CSF_FUNC(void*, __fastcall, LookupObjectById, 0x00652160, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static void __fastcall S_00537cd0_UpdateScenarioTransition(
    S_00537cd0* self, void* /*edx*/)
{
    if (CSF_AT(std::uint8_t, self, 0x186))
    {
        const int pendingIndex = CSF_AT(int, self, 0x144);
        void* globalObject = reinterpret_cast<void*>(GetGlobalValue_4FBDE0());
        void* pendingObject = GetGlobalSubObject_4FC0A0(globalObject, nullptr);
        if (IsPendingIndexMatch_443230(pendingObject, nullptr, pendingIndex))
        {
            CSF_AT(int, self, 0x144) = -1;
            CSF_AT(std::uint8_t, self, 0x186) = 0;
            CSF_AT(std::uint8_t, self, 0x185) = 1;
        }
    }

    reinterpret_cast<void (__fastcall*)(void*, void*)>(
        CSF_AT(void**, self, 0)[0xA0 / 4])(self, nullptr);

    if (!CSF_AT(std::uint8_t, self, 0x185))
        return;

    CSF_AT(std::uint8_t, self, 0x185) = 0;
    void* campaign = reinterpret_cast<void*>(GetGlobal8afc7c());
    if (Campaign_HasCurrentScene(campaign, nullptr))
    {
        void* menu = reinterpret_cast<void*>(GetGlobalValue_8B0568());
        const std::uint32_t capacity = CSF_AT(std::uint32_t, menu, 0x94);
        auto* sceneString = reinterpret_cast<std::uint8_t*>(menu) + 0x7C;
        char* sceneName;
        if (capacity < 0x10)
            sceneName = reinterpret_cast<char*>(sceneString + 4);
        else
            sceneName = CSF_AT(char*, sceneString, 4);

        campaign = reinterpret_cast<void*>(GetGlobal8afc7c());
        void* mission = reinterpret_cast<void*>(
            Campaign_FindMission(campaign, nullptr, sceneName));

        if (!mission)
        {
            CSF_AT(std::uint8_t, self, 0x181) = 1;
            return;
        }

        menu = reinterpret_cast<void*>(GetGlobalValue_8B0568());
        void* sceneLoader = UmsVirtualProcessor_GetDefaultDestination(menu, nullptr);
        SceneLoader_ClearAdditionalResources(sceneLoader, nullptr);

        char* missionName;
        if (CSF_AT(std::uint32_t, mission, 0x1C) < 0x10)
            missionName = reinterpret_cast<char*>(mission) + 8;
        else
            missionName = CSF_AT(char*, mission, 8);

        auto* selectedMenu = reinterpret_cast<S_004ff0f0*>(GetGlobalValue_8B0568());
        GlobalMenu_SetSelectedScenarioName(selectedMenu, nullptr, missionName, 1);
    }
    else
    {
        void* soundManager = reinterpret_cast<void*>(GetGlobalValue_521BB0());
        AudioSound_ForwardToGlobalObjectSlot29(soundManager, nullptr, 0);
        soundManager = reinterpret_cast<void*>(GetGlobalValue_521BB0());
        AudioSoundManager_FinishLoadContext(soundManager, nullptr);

        CSF_AT(int, self, 0x130) = 3;
        MissionScript_ResetPendingBlock(
            CSF_AT(void*, g_pCurrentScene, 0xBC), nullptr, 1);

        alignas(8) std::uint8_t menuBuffer[0xB4];
        MenuBuffer_Init(menuBuffer, nullptr);
        menuBuffer[25] = 1;
        menuBuffer[168] = 0;
        menuBuffer[26] = 0;

        void* gameState = reinterpret_cast<void*>(GetGlobalState_50D190());
        GameBddLoader_DispatchMenuRequest(
            gameState, nullptr, 1195716660, 1, 0,
            reinterpret_cast<int>(menuBuffer));

        gameState = reinterpret_cast<void*>(GetGlobalState_50D190());
        Interface_SetMenuState(gameState, nullptr, 4);

        gameState = reinterpret_cast<void*>(GetGlobalState_50D190());
        CSF_AT(int, gameState, 4) = 1;

        void* objectRegistry = reinterpret_cast<void*>(GetGlobalInt());
        void* object = LookupObjectById(objectRegistry, nullptr);
        reinterpret_cast<void (__fastcall*)(void*, void*)>(
            CSF_AT(void**, object, 0)[0x30 / 4])(object, nullptr);

        SceneLoader_DestroyMenuBuffer(
            reinterpret_cast<std::uint32_t*>(menuBuffer), nullptr);
    }
}

HOOK(0x00537A50, S_00537cd0_UpdateScenarioTransition, "auto");
