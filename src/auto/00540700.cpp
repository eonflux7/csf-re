#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Original callees, bound by address.
// Methods are written as __fastcall with ecx = this and edx unused, matching the
// disassembly (ecx loaded with the object, one argument pushed).
CSF_FUNC(void*, __fastcall, CMenuMissionInfo_BindPanelRegion, 0x540590, (void* self, void* edx, void* regionArg));
CSF_FUNC(void, __fastcall, CMenuMissionInfo_UpdatePanelLayout, 0x540530, (void* self, void* edx, void* panelArg));
CSF_FUNC(void, __fastcall, CMenuMissionInfo_RefreshPanelContent, 0x540480, (void* self, void* edx, void* panelArg));
CSF_FUNC(char, __fastcall, CMenuMissionInfo_ApplyPanelState, 0x53E930, (void* self, void* edx, void* panelArg));
CSF_FUNC(int, __fastcall, MissionInfoManager_GetSlotIndex, 0x515BD0, (void* self, void* edx));
CSF_FUNC(void*, __cdecl, GetGlobalValue_8B056C, 0x506BD0, ());

// Hooked target: refreshes all mission-info panels and dispatches a notification
// through the global manager's vtable slot 3.
static int __fastcall CMenuMissionInfo_RefreshAndDispatchPanels(void* self, void* /*edx*/)
{
    uint8_t* panelRegion = reinterpret_cast<uint8_t*>(self) + 0x30;

    CMenuMissionInfo_BindPanelRegion(self, nullptr, panelRegion);
    CMenuMissionInfo_UpdatePanelLayout(self, nullptr, panelRegion);
    CMenuMissionInfo_RefreshPanelContent(self, nullptr, panelRegion);
    CMenuMissionInfo_ApplyPanelState(self, nullptr, panelRegion);

    uint8_t* globalBlock = reinterpret_cast<uint8_t*>(GetGlobalValue_8B056C());
    void** manager = *reinterpret_cast<void***>(globalBlock + 8);
    void** managerVtbl = reinterpret_cast<void**>(manager[0]);
    int slotIndex = MissionInfoManager_GetSlotIndex(manager, nullptr);

    return ((int(__fastcall*)(void*, void*, int, void*, int))managerVtbl[3])(
        manager, nullptr, slotIndex, panelRegion, 1);
}

HOOK(0x540700, CMenuMissionInfo_RefreshAndDispatchPanels, "auto");
