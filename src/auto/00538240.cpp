#include <cstdint>
#include "globals.h"
#include <cstring>
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobal8afc7c, 0x0043E010, ());
CSF_FUNC(int, __fastcall, Campaign_FindMission, 0x0043E4B0, (void* self, void* , char* sceneName));
CSF_FUNC(bool, __fastcall, Campaign_HasCurrentScene, 0x0043E610, (void* self, void*));
CSF_FUNC(int*, __fastcall, Campaign_FlushPendingListEntry, 0x00441960, (void* self, void*));
CSF_FUNC(int, __fastcall, SceneLoader_ClearAdditionalResources, 0x0044C540, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_8B0568, 0x004FDBB0, ());
CSF_FUNC(void*, __fastcall, UmsVirtualProcessor_GetDefaultDestination, 0x004FE4A0, (void* self, void*));
CSF_FUNC(void, __fastcall, GlobalMenu_SetSelectedScenarioName, 0x00501990, (S_004ff0f0* self, void* , char* String1, char a3));
CSF_FUNC(char, __fastcall, S_00537cd0_UpdateMultiplayerState, 0x00535ED0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, S_00537cd0_UpdateMultiplayerControls, 0x00535F60, (void* self, void* edx));
CSF_FUNC(int, __fastcall, S_00537cd0_UpdateWithoutCurrentScene, 0x00536010, (void* self, void* edx));
CSF_FUNC(char, __fastcall, S_00537cd0_UpdateMultiplayerSelection, 0x005369A0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, Sub_538190, 0x00538190, (int self, void* edx));
CSF_FUNC(int, __fastcall, MenuMpCharSelect_Update, 0x005428A0, (void* self, void* edx, float a2));

static void __fastcall S_00537cd0_UpdateMissionSelection(
    S_00537cd0* self, void* /*edx*/, S_0054ecf0* updateArgument)
{
    void* globalMenu = reinterpret_cast<void*>(GetGlobalValue_8B0568());
    void* destination =
        UmsVirtualProcessor_GetDefaultDestination(globalMenu, nullptr);

    if (CSF_AT(uint8_t, self, 0x181))
    {
        if (CSF_AT(uint32_t, destination, 0x1C))
        {
            void* campaign = reinterpret_cast<void*>(GetGlobal8afc7c());
            Campaign_FlushPendingListEntry(campaign, nullptr);
        }
        Sub_538190(reinterpret_cast<int>(self), nullptr);
        return;
    }

    const uint32_t destinationState = CSF_AT(uint32_t, destination, 0x1C);
    if (destinationState != 1 && destinationState != 0)
    {
        S_00537cd0_UpdateMultiplayerState(self, nullptr);
        S_00537cd0_UpdateMultiplayerControls(self, nullptr);
        S_00537cd0_UpdateMultiplayerSelection(self, nullptr);

        // The original forwards the argument's raw stack bits to a float callee.
        float updateValue;
        static_assert(sizeof(updateValue) == sizeof(updateArgument));
        std::memcpy(&updateValue, &updateArgument, sizeof(updateValue));
        MenuMpCharSelect_Update(
            reinterpret_cast<void*>(self->field_014c), nullptr, updateValue);

        reinterpret_cast<void (__fastcall*)(S_00537cd0*, void*, S_0054ecf0*)>(
            CSF_AT(void**, self, 0)[0x84 / 4])(
                self, nullptr, updateArgument);
        return;
    }

    void* campaign = reinterpret_cast<void*>(GetGlobal8afc7c());
    if (!Campaign_HasCurrentScene(campaign, nullptr))
    {
        S_00537cd0_UpdateWithoutCurrentScene(self, nullptr);
        return;
    }

    globalMenu = reinterpret_cast<void*>(GetGlobalValue_8B0568());
    const uint32_t sceneCapacity = CSF_AT(uint32_t, globalMenu, 0x94);
    auto* sceneString = reinterpret_cast<uint8_t*>(globalMenu) + 0x7C;
    char* sceneName = sceneCapacity < 0x10
        ? reinterpret_cast<char*>(sceneString + 4)
        : CSF_AT(char*, sceneString, 4);

    campaign = reinterpret_cast<void*>(GetGlobal8afc7c());
    void* mission = reinterpret_cast<void*>(
        Campaign_FindMission(campaign, nullptr, sceneName));
    if (!mission)
    {
        CSF_AT(uint8_t, self, 0x181) = 1;
        return;
    }

    globalMenu = reinterpret_cast<void*>(GetGlobalValue_8B0568());
    destination = UmsVirtualProcessor_GetDefaultDestination(globalMenu, nullptr);
    SceneLoader_ClearAdditionalResources(destination, nullptr);

    char* missionName = CSF_AT(uint32_t, mission, 0x1C) < 0x10
        ? reinterpret_cast<char*>(mission) + 8
        : CSF_AT(char*, mission, 8);

    auto* menu = reinterpret_cast<S_004ff0f0*>(GetGlobalValue_8B0568());
    GlobalMenu_SetSelectedScenarioName(menu, nullptr, missionName, 1);
}

HOOK(0x00538240, S_00537cd0_UpdateMissionSelection, "auto");
