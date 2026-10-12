#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobal8afc7c, 0x0043E010, ());
CSF_FUNC(int, __fastcall, Campaign_FindMission, 0x0043E4B0, (void* self, void* , char* sceneName));
CSF_FUNC(int, __cdecl, GetGlobalValue_8B0568, 0x004FDBB0, ());
CSF_FUNC(int, __fastcall, S_00500350_GetFirstRecordNameBuffer, 0x00500350, (S_00500350* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalValue_8B056C, 0x00506BD0, ());
CSF_FUNC(int*, __fastcall, ProfileSavegames_GetScenarioCollection, 0x00515CE0, (void* self, void*));

static int __cdecl MenuMissionInfo_GetCurrentMissionScenarioValue()
{
    void* manager = reinterpret_cast<void*>(GetGlobalValue_8B056C());
    void* profile = CSF_AT(void*, manager, 0x08);
    if (!profile)
        return 0;

    int* scenarios = ProfileSavegames_GetScenarioCollection(profile, nullptr);
    const uint32_t scenarioBegin = CSF_AT(uint32_t, scenarios, 0x04);
    if (!scenarioBegin)
        return 0;

    const int32_t scenarioBytes = static_cast<int32_t>(
        CSF_AT(uint32_t, scenarios, 0x08) - scenarioBegin);
    if (scenarioBytes / 80 == 0)
        return 0;

    void* missionData =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, profile, 0)[0x24 / 4])(profile, nullptr);

    const uint32_t entryBegin = CSF_AT(uint32_t, missionData, 0x38);
    const uint32_t entryCount = entryBegin
        ? static_cast<uint32_t>(
            static_cast<int32_t>(
                CSF_AT(uint32_t, missionData, 0x3C) - entryBegin) >> 4)
        : 0;

    S_00500350* records =
        reinterpret_cast<S_00500350*>(GetGlobalValue_8B0568());

    // Retain the packet's vtable binding; this address is also the direct callee.
    (void)CSF_VTBL(0x00500350);
    char* sceneName = reinterpret_cast<char*>(
        S_00500350_GetFirstRecordNameBuffer(records, nullptr));
    void* campaign = reinterpret_cast<void*>(GetGlobal8afc7c());
    void* mission = reinterpret_cast<void*>(
        Campaign_FindMission(campaign, nullptr, sceneName));
    if (!mission)
        return 0;

    for (uint32_t index = 0, offset = 0;
         index < entryCount;
         ++index, offset += 16)
    {
        const uint32_t entries = CSF_AT(uint32_t, missionData, 0x38);
        const uint32_t missionId = CSF_AT(uint32_t, mission, 0);
        if (missionId == CSF_AT(uint32_t,
                               reinterpret_cast<void*>(entries + offset), 0))
        {
            const uint32_t currentEntries =
                CSF_AT(uint32_t, missionData, 0x38);
            return CSF_AT(int, reinterpret_cast<void*>(
                currentEntries + 16 * index + 4), 0);
        }
    }
    return 0;
}

HOOK(0x0053DE30, MenuMissionInfo_GetCurrentMissionScenarioValue, "auto");
