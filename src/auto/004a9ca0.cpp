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

CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(uint32_t*, __fastcall, WaypointGraph_GetGridSlotPosition, 0x0075D000, (S_004abb90* self, void* /*edx*/, uint32_t* a2));

static uint32_t* __fastcall AiCoverAttackTasks_GetPositionData(S_004abb90* self, void* /*edx*/)
{
    // Unset sentinel (-1) in either the first dword or field_0004: return the inline slot at +8.
    if (CSF_AT(int32_t, self, 0x0) == -1 || self->field_0004 == 0xFFFFFFFFu)
        return &self->field_0008;

    // Otherwise ask the global state for its waypoint graph (stored at +0x1FD8) and resolve the grid slot.
    int stateAddress = GetValue8B0554();
    uint8_t* stateBase = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(stateAddress));
    S_004abb90* waypointGraph = CSF_AT(S_004abb90*, stateBase, 0x1FD8);
    return WaypointGraph_GetGridSlotPosition(waypointGraph, nullptr, reinterpret_cast<uint32_t*>(self));
}

HOOK(0x004A9CA0, AiCoverAttackTasks_GetPositionData, "auto");
