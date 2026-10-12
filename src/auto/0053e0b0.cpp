#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobal8afc7c, 0x0043E010, ());
CSF_FUNC(int, __fastcall, Campaign_FindMission, 0x0043E4B0, (void* self, void* , char* sceneName));
CSF_FUNC(int, __cdecl, MissionInfo_IsPrerequisiteMet, 0x0044FB00, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_8B0568, 0x004FDBB0, ());
CSF_FUNC(void*, __fastcall, UmsVirtualProcessor_GetDefaultDestination, 0x004FE4A0, (void* self, void*));
CSF_FUNC(int, __fastcall, S_00500350_GetFirstRecordNameBuffer, 0x00500350, (S_00500350* self, void*));
CSF_FUNC(int, __fastcall, MenuMissionInfo_ComputeScaledLevel, 0x0053DB70, (void* self, void*));

static char __fastcall CMenuOptions_CanSelectMission(
    CMenuOptions* self, void* /*edx*/, int recordsAddress)
{
    // Retain the packet's vtable annotation for this callee address.
    (void)CSF_VTBL(0x00500350);

    auto* state = reinterpret_cast<S_00500350*>(GetGlobalValue_8B0568());
    char* sceneName = reinterpret_cast<char*>(
        S_00500350_GetFirstRecordNameBuffer(state, nullptr));
    void* campaign = reinterpret_cast<void*>(GetGlobal8afc7c());
    void* mission = reinterpret_cast<void*>(
        Campaign_FindMission(campaign, nullptr, sceneName));

    void* records = reinterpret_cast<void*>(recordsAddress);
    uint32_t begin = CSF_AT(uint32_t, records, 0x38);
    uint32_t recordCount = 0;
    if (begin)
    {
        recordCount = static_cast<uint32_t>(
            static_cast<int32_t>(
                CSF_AT(uint32_t, records, 0x3C) - begin) >> 4);
    }

    bool complete = false;
    if (!CSF_AT(uint8_t, records, 0x44))
    {
        void* currentCampaign = reinterpret_cast<void*>(GetGlobal8afc7c());
        uint32_t missionBegin = CSF_AT(uint32_t, currentCampaign, 0x34);
        int32_t missionCount = 0;
        if (missionBegin)
        {
            missionCount = static_cast<int32_t>(
                CSF_AT(uint32_t, currentCampaign, 0x38) - missionBegin) / 196;
        }
        complete = recordCount == static_cast<uint32_t>(missionCount);
    }

    if (!complete)
        return 0;

    uint32_t matchingCount = 0;
    uint32_t index = 0;
    uint32_t byteOffset = 0;
    int missionId = -1;
    while (matchingCount < 2 && index < recordCount)
    {
        auto* record = reinterpret_cast<uint8_t*>(
            CSF_AT(uint32_t, records, 0x38)) + byteOffset;
        if (CSF_AT(int, record, 0x0C) != 5)
        {
            missionId = CSF_AT(int, record, 0x00);
            ++matchingCount;
        }
        ++index;
        byteOffset += 16;
    }

    if (matchingCount != 1 || !mission ||
        missionId != CSF_AT(int, mission, 0x00))
        return 0;

    if (!static_cast<uint8_t>(MissionInfo_IsPrerequisiteMet()))
        return 0;

    void* currentState = reinterpret_cast<void*>(GetGlobalValue_8B0568());
    if (!UmsVirtualProcessor_GetDefaultDestination(currentState, nullptr))
        return 0;

    currentState = reinterpret_cast<void*>(GetGlobalValue_8B0568());
    void* destination =
        UmsVirtualProcessor_GetDefaultDestination(currentState, nullptr);
    if (CSF_AT(uint32_t, destination, 0x1C))
        return 0;

    return MenuMissionInfo_ComputeScaledLevel(self, nullptr) == 5 ? 1 : 0;
}

HOOK(0x0053E0B0, CMenuOptions_CanSelectMission, "auto");
