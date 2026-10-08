#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Returns the global object pointer used as the lookup source (sub_43E010).
CSF_FUNC(void*, __cdecl, GetGlobal8afc7c, 0x0043E010, ());

// __thiscall on the global object; returns a derived key pointer (sub_43E5B0).
CSF_FUNC(uint32_t*, __fastcall, ResolveCurrentPanelKey, 0x0043E5B0, (void* self, void* edx));

// __thiscall lookup on the global object with one stack argument (sub_43E4B0).
CSF_FUNC(uint32_t*, __fastcall, MenuLoading_FindMission, 0x0043E4B0, (void* self, void* edx, uint32_t* key));

// __thiscall on the panel's +0x34 sub-object with one stack argument (sub_516530).
CSF_FUNC(unsigned int, __fastcall, MissionInfoPanel_AddRecord, 0x00516530, (void* self, void* edx, uint32_t* record));

// __stdcall validity check on the menu object (sub_53DFA0).
CSF_FUNC(char, __stdcall, CMenuMissionInfo_IsCurrentEntryAbsentFromList, 0x0053DFA0, (int a1));

static char __stdcall CMenuMissionInfo_AddRecordFromGlobalLookup(int a1)
{
    if (CMenuMissionInfo_IsCurrentEntryAbsentFromList(a1) == 0)
    {
        return 0;
    }

    CMenuMissionInfo* self = reinterpret_cast<CMenuMissionInfo*>(a1);

    void* globalLookupSource = GetGlobal8afc7c();
    uint32_t* lookupKey = ResolveCurrentPanelKey(globalLookupSource, nullptr);

    void* globalLookupTarget = GetGlobal8afc7c();
    uint32_t* foundEntry = MenuLoading_FindMission(globalLookupTarget, nullptr, lookupKey);

    if (foundEntry == nullptr)
    {
        return 0;
    }

    uint32_t record[4];
    record[0] = *foundEntry;
    record[1] = 0;
    record[2] = 0;
    record[3] = 0;

    return static_cast<char>(MissionInfoPanel_AddRecord(&self->field_0034, nullptr, record));
}

HOOK(0x00540530, CMenuMissionInfo_AddRecordFromGlobalLookup, "auto");
