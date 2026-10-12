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

static char __cdecl MenuMissionInfo_GetSelectedMissionFlag()
{
    // Retain the packet's vtable reference without dereferencing it.
    (void)CSF_VTBL(0x00500350);

    char result = 0;
    void* manager = reinterpret_cast<void*>(GetGlobalValue_8B056C());
    void* profile = reinterpret_cast<void*>(CSF_AT(uint32_t, manager, 0x08));
    if (!profile)
        return 0;

    int* scenarios = ProfileSavegames_GetScenarioCollection(profile, nullptr);
    uint32_t scenarioBegin = CSF_AT(uint32_t, scenarios, 0x04);
    if (!scenarioBegin)
        return 0;

    int32_t scenarioBytes = static_cast<int32_t>(
        CSF_AT(uint32_t, scenarios, 0x08) - scenarioBegin);
    if (scenarioBytes / 80 == 0)
        return 0;

    void* records = reinterpret_cast<void*>(
        reinterpret_cast<int(__fastcall*)(void*, void*)>(
            CSF_AT(void**, profile, 0)[0x24 / 4])(profile, nullptr));

    uint32_t recordBegin = CSF_AT(uint32_t, records, 0x38);
    uint32_t recordCount = 0;
    if (recordBegin)
    {
        int32_t recordBytes = static_cast<int32_t>(
            CSF_AT(uint32_t, records, 0x3C) - recordBegin);
        recordCount = static_cast<uint32_t>(recordBytes >> 4);
    }

    S_00500350* selection =
        reinterpret_cast<S_00500350*>(GetGlobalValue_8B0568());
    char* sceneName = reinterpret_cast<char*>(
        S_00500350_GetFirstRecordNameBuffer(selection, nullptr));
    void* campaign = reinterpret_cast<void*>(GetGlobal8afc7c());
    void* mission = reinterpret_cast<void*>(
        Campaign_FindMission(campaign, nullptr, sceneName));

    if (mission)
    {
        uint32_t offset = 0;
        for (uint32_t index = 0; index < recordCount; ++index, offset += 16)
        {
            uint32_t currentBegin = CSF_AT(uint32_t, records, 0x38);
            uint32_t missionId = CSF_AT(uint32_t, mission, 0);
            void* record = reinterpret_cast<void*>(currentBegin + offset);
            if (missionId == CSF_AT(uint32_t, record, 0))
                result = CSF_AT(char, record, 0x08);
        }
    }

    return result;
}

HOOK(0x0053E230, MenuMissionInfo_GetSelectedMissionFlag, "auto");
