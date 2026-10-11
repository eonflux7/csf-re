#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "types/auto_classes.h"

// Original sub_75CF20 at 0x75CF20: ecx = this, one stack argument (a2), returns eax.
CSF_FUNC(int, __fastcall, WaypointGraph_ComputeGridSlotIndex, 0x0075CF20, (void* self, void* edx, uint32_t* a2));

// __thiscall-style hook: ecx = this, one stack argument, ret 4.
static int __fastcall WaypointGraph_GetGridSlotPositionAdjusted(C_0086c288* self, void* /*edx*/, uint32_t* a2) {
    return WaypointGraph_ComputeGridSlotIndex(self, nullptr, a2) + 28;
}

HOOK(0x0075D0C0, WaypointGraph_GetGridSlotPositionAdjusted, "auto");
