// Module: bdd_waypoint_graph
// Copies the seven leading dwords of a waypoint record into *this, then
// copies the trailing sub-record at +0x1C through a nested callee.

#include <cstdint>
#include "globals.h"
#include "hook.h"

// Callee at 0x0075BF70: __thiscall, one stack arg, ret 4.
CSF_FUNC(uint32_t*, __fastcall, WaypointGraph_CopyRecordTail, 0x0075BF70,
         (void* self, void* edx, uint32_t* a2));

static uint32_t* __fastcall WaypointGraph_CopyRecord(void* self, void* /*edx*/, uint32_t* a2) {
    uint32_t* dst = static_cast<uint32_t*>(self);

    dst[0] = a2[0];
    dst[1] = a2[1];
    dst[2] = a2[2];
    dst[3] = a2[3];
    dst[4] = a2[4];
    dst[5] = a2[5];
    dst[6] = a2[6];

    // Trailing sub-record at offset 0x1C in both source and destination.
    WaypointGraph_CopyRecordTail(dst + 7, nullptr, a2 + 7);
    return dst;
}

HOOK(0x0075C270, WaypointGraph_CopyRecord, "auto");
