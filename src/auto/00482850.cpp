#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());
CSF_FUNC(int, __cdecl, GetGlobal8B0558, 0x004F4920, ());
CSF_FUNC(void, __fastcall, SetGlobalNameWithValue, 0x004F49A0, (void* self, void* edx, int a4, float a5));
CSF_FUNC(int, __cdecl, Get_008b0678, 0x00525130, ());
CSF_FUNC(int, __fastcall, MenuMissionInfo_SetGlobalIndex, 0x00525DD0, (void* self, void* edx, int value));

// Selects the mission briefing global for slots 1..5, applies its float setting,
// then sets the menu mission global index to 91. Slots outside 1..5 return slot - 1.
static int __stdcall CharActionFsm_SelectMissionBriefing(int slot)
{
    int result = slot - 1;
    const char* missionGlobalName = nullptr;

    switch (slot)
    {
    case 1:
        missionGlobalName = CSF_STR(0x83345C, "g077");
        break;
    case 2:
        missionGlobalName = CSF_STR(0x833454, "g078");
        break;
    case 3:
        missionGlobalName = CSF_STR(0x83344C, "g079");
        break;
    case 4:
        missionGlobalName = CSF_STR(0x833444, "g080");
        break;
    case 5:
        missionGlobalName = CSF_STR(0x83343C, "g081");
        break;
    default:
        return result;
    }

    uint8_t* globalsBase = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(GetGlobalValue_8BC1E0()));
    float briefingValue = CSF_AT(float, globalsBase, 0x18DC);

    SetGlobalNameWithValue(reinterpret_cast<void*>(static_cast<uintptr_t>(GetGlobal8B0558())),
                           nullptr,
                           reinterpret_cast<int>(missionGlobalName),
                           briefingValue);

    result = MenuMissionInfo_SetGlobalIndex(
        reinterpret_cast<void*>(static_cast<uintptr_t>(Get_008b0678())),
        nullptr,
        91);
    return result;
}

HOOK(0x00482850, CharActionFsm_SelectMissionBriefing, "auto");
