#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// __cdecl, no stack args; returns the global root pointer.
CSF_FUNC(int, __cdecl, GetGlobalValue_8B056C, 0x506BD0, ());

// __thiscall, no stack args; returns a pointer to the objective entry list header.
CSF_FUNC(int*, __fastcall, GetObjectiveEntryList, 0x515CE0, (int* self, void* edx));

// __thiscall, one stack arg (callee pops 4); assigns the value to the subpanel at this+0x30.
CSF_FUNC(int, __fastcall, ObjectivePanel_Assign, 0x5150F0, (void* self, void* edx, int value));

static char __fastcall CMenuMissionInfo_LoadObjectiveFromMissionData(CMenuMissionInfo* self, void* /*edx*/)
{
    // Root global -> mission data object at +8.
    int globalRoot = GetGlobalValue_8B056C();
    int* missionData = *reinterpret_cast<int**>(static_cast<uintptr_t>(globalRoot) + 8);

    // Entry list header: [1] = begin, [2] = end, entries are 80 bytes each.
    int* entryList = GetObjectiveEntryList(missionData, nullptr);
    int entriesBegin = entryList[1];
    if (!entriesBegin)
        return 0;

    int32_t entriesSpan = static_cast<int32_t>(static_cast<uint32_t>(entryList[2]) -
                                               static_cast<uint32_t>(entriesBegin));
    if (!(entriesSpan / 80))
        return 0;

    // Virtual slot 9 (offset 0x24) on the mission data object.
    int selectedValue = reinterpret_cast<int(__fastcall*)(void*, void*)>(CSF_AT(void**, missionData, 0)[9])(missionData, nullptr);

    ObjectivePanel_Assign(reinterpret_cast<uint8_t*>(self) + 0x30, nullptr, selectedValue);
    return 1;
}

HOOK(0x005406A0, CMenuMissionInfo_LoadObjectiveFromMissionData, "auto");
