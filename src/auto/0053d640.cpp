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

// Original functions called by this routine.
CSF_FUNC(int, __cdecl, GetGlobalValue_75FDF0, 0x0075FDF0, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_8B0568, 0x004FDBB0, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_8B056C, 0x00506BD0, ());
CSF_FUNC(int, __fastcall, ComputeChildHandleFromSlot, 0x00515BD0, (void* self, void* edx));

// Sends the mission-info panel's sub-object handle (this+0x30) to the
// manager object's virtual slot 3 when the game-state flags allow it.
// Returns the result of the last evaluated call, as the original does.
static int __fastcall CMenuMissionInfo_SyncPanelWithMissionState(CMenuMissionInfo* self, void* /*edx*/)
{
    int gameStateA = GetGlobalValue_75FDF0();
    if (CSF_AT(uint8_t, reinterpret_cast<void*>(gameStateA), 0xCC))
    {
        int gameStateB = GetGlobalValue_8B0568();
        if (!CSF_AT(uint8_t, reinterpret_cast<void*>(gameStateB), 0xB4))
        {
            int managerRef = GetGlobalValue_8B056C();
            void* panelOwner = reinterpret_cast<void*>(CSF_AT(uint32_t, reinterpret_cast<void*>(managerRef), 8));
            void** panelVtbl = CSF_AT(void**, panelOwner, 0);
            int childHandle = ComputeChildHandleFromSlot(panelOwner, nullptr);
            void* panelRegion = reinterpret_cast<uint8_t*>(self) + 0x30;
            return ((int(__fastcall*)(void*, void*, int, void*, int))panelVtbl[3])(
                panelOwner, nullptr, childHandle, panelRegion, 0);
        }
    }
    return gameStateA;
}

HOOK(0x0053D640, CMenuMissionInfo_SyncPanelWithMissionState, "auto");
