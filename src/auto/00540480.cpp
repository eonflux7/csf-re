#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobal8afc7c, 0x0043E010, ());
CSF_FUNC(int, __fastcall, Campaign_FindMission, 0x0043E4B0, (void* self, void* , char* sceneName));
CSF_FUNC(int, __cdecl, GetGlobalValue_8B0568, 0x004FDBB0, ());
CSF_FUNC(int, __fastcall, S_00500350_GetFirstRecordNameBuffer, 0x00500350, (S_00500350* self, void*));
CSF_FUNC(unsigned int, __fastcall, MissionInfoPanel_AddRecord, 0x00516530, (S_00514aa0* self, void* , uint32_t* source));
CSF_FUNC(bool, __fastcall, CMenuMissionInfo_TryHandleTertiaryPanel, 0x0053DEF0, (CMenuMissionInfo* self, void* edx, int a1));

[[maybe_unused]] static const auto kRecordNameBufferVtable = CSF_VTBL(0x00500350);

static char __stdcall CMenuMissionInfo_RefreshPanelContent(int panelAddress)
{
    // The bound callee ignores its register arguments.
    if (!CMenuMissionInfo_TryHandleTertiaryPanel(nullptr, nullptr, panelAddress))
        return 0;

    auto* globalState = reinterpret_cast<S_00500350*>(GetGlobalValue_8B0568());
    auto* sceneName = reinterpret_cast<char*>(
        S_00500350_GetFirstRecordNameBuffer(globalState, nullptr));
    auto* campaign = reinterpret_cast<void*>(GetGlobal8afc7c());
    auto* mission = reinterpret_cast<uint32_t*>(
        Campaign_FindMission(campaign, nullptr, sceneName));
    if (!mission)
        return 0;

    auto* panel = reinterpret_cast<CMenuMissionInfo*>(panelAddress);
    auto* records = reinterpret_cast<S_00514aa0*>(
        reinterpret_cast<uint8_t*>(panel) + 0x34);

    const uint32_t beginAddress = CSF_AT(uint32_t, panel, 0x38);
    uint32_t count = 0;
    if (beginAddress)
    {
        const uint32_t byteCount =
            CSF_AT(uint32_t, panel, 0x3C) - beginAddress;
        count = static_cast<uint32_t>(static_cast<int32_t>(byteCount) >> 4);
    }

    if (count)
    {
        auto* begin = reinterpret_cast<uint8_t*>(
            CSF_AT(uint32_t, panel, 0x38));
        const uint32_t missionKey = CSF_AT(uint32_t, mission, 0);
        auto* entry = begin;
        for (uint32_t index = 0; index < count; ++index, entry += 0x10)
        {
            if (missionKey == CSF_AT(uint32_t, entry, 0))
            {
                CSF_AT(uint8_t, entry, 8) = 1;
                // This path leaves the mission key in EAX, not a success flag.
                return static_cast<char>(missionKey);
            }
        }
    }

    alignas(8) uint8_t recordBytes[0x10] = {};
    CSF_AT(uint32_t, recordBytes, 0) = CSF_AT(uint32_t, mission, 0);
    CSF_AT(uint32_t, recordBytes, 4) = 0;
    CSF_AT(uint32_t, recordBytes, 0xC) = 0;
    CSF_AT(uint8_t, recordBytes, 8) = 1;
    return static_cast<char>(MissionInfoPanel_AddRecord(
        records, nullptr, reinterpret_cast<uint32_t*>(recordBytes)));
}

HOOK(0x00540480, CMenuMissionInfo_RefreshPanelContent, "auto");
