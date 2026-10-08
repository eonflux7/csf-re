#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"

// Callees, bound by address.
CSF_FUNC(char, __fastcall, CMenuMissionInfo_LoadObjectiveFromMissionData, 0x005406A0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CMenuMissionInfo_CheckAdvanceCondition, 0x0053E040, (void* self, void* edx, void* subPanel));
CSF_FUNC(int, __fastcall, CMenuMissionInfo_RefreshAndDispatchPanels, 0x00540700, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CMenuMissionInfo_SyncPanelWithMissionState, 0x0053D640, (void* self, void* edx));
// Original takes a2 in edi (__userpurge); see notes in metadata.
CSF_FUNC(void, __fastcall, CMenuMissionInfo_BaseHandleEvent, 0x0053DC60, (void* self, void* edx, char a3));

// Refreshes the mission panel when it is active and ready, then forwards to the base handler.
static void __fastcall CMenuMissionInfo_UpdateAndForward(void* self, void* /*edx*/, char a3)
{
    if (CMenuMissionInfo_LoadObjectiveFromMissionData(self, nullptr))
    {
        void* subPanel = static_cast<uint8_t*>(self) + 0x30;
        if (CMenuMissionInfo_CheckAdvanceCondition(self, nullptr, subPanel))
        {
            CMenuMissionInfo_RefreshAndDispatchPanels(self, nullptr);
            CMenuMissionInfo_SyncPanelWithMissionState(self, nullptr);
        }
    }
    CMenuMissionInfo_BaseHandleEvent(self, nullptr, a3);
}

HOOK(0x00540750, CMenuMissionInfo_UpdateAndForward, "auto");
