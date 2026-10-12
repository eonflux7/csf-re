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
CSF_FUNC(unsigned int, __fastcall, MissionInfoPanel_AddRecord, 0x00516530, (S_00514aa0* self, void* , uint32_t* source));
CSF_FUNC(int, __fastcall, MenuMissionInfo_ComputeScaledLevel, 0x0053DB70, (void* self, void*));

static uint32_t __fastcall CMenuMissionInfo_BindPanelRegion(
    CMenuMissionInfo* self, void* /*edx*/, void* panel)
{
    // The packet also identifies this callee address as a vtable reference.
    (void)CSF_VTBL(0x00500350);

    uint32_t result = self->field_0090;
    if (result >= self->field_008c)
        return result;

    result = static_cast<uint32_t>(MissionInfo_IsPrerequisiteMet());
    if (static_cast<uint8_t>(result) == 0)
        return result;

    void* context = reinterpret_cast<void*>(GetGlobalValue_8B0568());
    void* destination = UmsVirtualProcessor_GetDefaultDestination(context, nullptr);
    if (!destination)
        return 0;

    context = reinterpret_cast<void*>(GetGlobalValue_8B0568());
    destination = UmsVirtualProcessor_GetDefaultDestination(context, nullptr);
    if (CSF_AT(uint32_t, destination, 0x1C) != 0)
        return reinterpret_cast<uint32_t>(destination);

    auto* records = reinterpret_cast<S_00500350*>(GetGlobalValue_8B0568());
    char* sceneName = reinterpret_cast<char*>(
        S_00500350_GetFirstRecordNameBuffer(records, nullptr));
    void* campaign = reinterpret_cast<void*>(GetGlobal8afc7c());
    result = static_cast<uint32_t>(
        Campaign_FindMission(campaign, nullptr, sceneName));
    if (result == 0)
        return result;

    void* mission = reinterpret_cast<void*>(result);
    uint32_t begin = CSF_AT(uint32_t, panel, 0x38);
    auto* recordList = reinterpret_cast<S_00514aa0*>(
        reinterpret_cast<uint8_t*>(panel) + 0x34);

    uint32_t count = 0;
    if (begin != 0)
    {
        uint32_t difference = CSF_AT(uint32_t, panel, 0x3C) - begin;
        count = static_cast<uint32_t>(static_cast<int32_t>(difference) >> 4);
    }

    if (count != 0)
    {
        uint8_t* firstRecord = reinterpret_cast<uint8_t*>(
            CSF_AT(uint32_t, panel, 0x38));
        const uint32_t missionId = CSF_AT(uint32_t, mission, 0);
        uint8_t* record = firstRecord;

        for (uint32_t index = 0; index < count; ++index, record += 0x10)
        {
            if (CSF_AT(uint32_t, record, 0) == missionId)
            {
                CSF_AT(uint32_t, record, 0x4) = self->field_008c;
                result = static_cast<uint32_t>(
                    MenuMissionInfo_ComputeScaledLevel(self, nullptr));
                CSF_AT(uint32_t, record, 0xC) = result;
                return result;
            }
        }
    }

    alignas(8) uint8_t recordBytes[0x18] = {};
    CSF_AT(uint32_t, recordBytes, 0x4) = self->field_008c;
    CSF_AT(uint32_t, recordBytes, 0) = CSF_AT(uint32_t, mission, 0);
    CSF_AT(uint8_t, recordBytes, 0x8) = 0;
    CSF_AT(uint32_t, recordBytes, 0x10) = reinterpret_cast<uint32_t>(mission);
    CSF_AT(uint32_t, recordBytes, 0x14) = reinterpret_cast<uint32_t>(self);
    CSF_AT(uint32_t, recordBytes, 0xC) = static_cast<uint32_t>(
        MenuMissionInfo_ComputeScaledLevel(self, nullptr));

    return MissionInfoPanel_AddRecord(
        recordList, nullptr, reinterpret_cast<uint32_t*>(recordBytes));
}

HOOK(0x00540590, CMenuMissionInfo_BindPanelRegion, "auto");
