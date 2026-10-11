#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original callees, bound by address.
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(int, __fastcall, C_0086c288_ClearNodeContainers, 0x0075D1F0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_0082d410_ReleaseBaseState, 0x0075E070, (void* self, void* edx));
CSF_FUNC(char, __fastcall, WaypointGraph_ReleaseNodeBuffer, 0x0075F9B0, (int a1, int a2));

// Releases the waypoint graph owned by this object and resets it to its base-class state.
static int __fastcall C_0086c288_ReleaseWaypointGraph(C_0086c288* self, void* /*edx*/)
{
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x0086C288);

    C_0086c288_ClearNodeContainers(self, nullptr);

    uint32_t nodeBuffer = self->field_0014;
    if (nodeBuffer)
    {
        WaypointGraph_ReleaseNodeBuffer(static_cast<int>(nodeBuffer), 0);
        Mem_Free(reinterpret_cast<void*>(static_cast<uintptr_t>(nodeBuffer)));
        self->field_0014 = 0;
    }

    if (self->field_0020)
    {
        Mem_Free(reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0020)));
        self->field_0020 = 0;
    }

    int result = C_0082d410_ReleaseBaseState(reinterpret_cast<uint8_t*>(self) + 4, nullptr);

    *reinterpret_cast<void***>(self) = CSF_VTBL(0x0082D410);
    return result;
}

HOOK(0x75E1E0, C_0086c288_ReleaseWaypointGraph, "auto");
