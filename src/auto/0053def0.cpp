#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobal8afc7c, 0x0043E010, ());
CSF_FUNC(int, __fastcall, Campaign_FindMission, 0x0043E4B0, (void* self, void* , char* sceneName));
CSF_FUNC(int, __cdecl, MissionInfo_IsPrerequisiteMet, 0x0044FB00, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_8B0568, 0x004FDBB0, ());
CSF_FUNC(void*, __fastcall, UmsVirtualProcessor_GetDefaultDestination, 0x004FE4A0, (void* self, void*));
CSF_FUNC(int, __fastcall, S_00500350_GetFirstRecordNameBuffer, 0x00500350, (S_00500350* self, void*));

static int __stdcall MenuMissionInfo_CanUseTertiaryPanel(int panelAddress)
{
    // Retain the address binding classified as a vtable in the packet.
    (void)CSF_VTBL(0x00500350);

    if (static_cast<uint8_t>(MissionInfo_IsPrerequisiteMet()) == 0)
        return 0;

    void* state = reinterpret_cast<void*>(GetGlobalValue_8B0568());
    if (!UmsVirtualProcessor_GetDefaultDestination(state, nullptr))
        return 0;

    state = reinterpret_cast<void*>(GetGlobalValue_8B0568());
    void* destination = UmsVirtualProcessor_GetDefaultDestination(state, nullptr);
    if (CSF_AT(uint32_t, destination, 0x1C) != 0)
        return 0;

    S_00500350* recordsOwner =
        reinterpret_cast<S_00500350*>(GetGlobalValue_8B0568());
    char* sceneName = reinterpret_cast<char*>(
        S_00500350_GetFirstRecordNameBuffer(recordsOwner, nullptr));

    void* campaign = reinterpret_cast<void*>(GetGlobal8afc7c());
    void* mission = reinterpret_cast<void*>(
        Campaign_FindMission(campaign, nullptr, sceneName));
    if (!mission)
        return 0;

    void* panel = reinterpret_cast<void*>(panelAddress);
    const uint32_t beginAddress = CSF_AT(uint32_t, panel, 0x38);
    uint32_t recordCount = 0;
    if (beginAddress != 0)
    {
        const uint32_t byteCount =
            CSF_AT(uint32_t, panel, 0x3C) - beginAddress;
        recordCount = static_cast<uint32_t>(
            static_cast<int32_t>(byteCount) >> 4);
    }

    if (recordCount == 0)
        return 1;

    uint8_t* records =
        reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, panel, 0x38));
    const uint32_t missionId = CSF_AT(uint32_t, mission, 0);

    for (uint32_t index = 0; index < recordCount; ++index)
    {
        uint8_t* record = records + index * 0x10;
        if (CSF_AT(uint32_t, record, 0) == missionId)
            return CSF_AT(uint8_t, record, 8) == 0;
    }

    return 1;
}

HOOK(0x0053DEF0, MenuMissionInfo_CanUseTertiaryPanel, "auto");
