#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(int, __cdecl, WaypointGraph_DestroyEdgeSet, 0x0075D0E0, (int a1, int a2, int a3, int a4));

// Releases the edge-set buffers and the two heap blocks owned by a waypoint graph
// object, then resets its bookkeeping fields to the empty state.
static int __fastcall WaypointGraph_ClearStorage(void* self, void* /*edx*/) {
    // Edge set header lives at +0x34; its root/size fields are at +0x38/+0x3C.
    int edgeRoot = CSF_AT(int, self, 0x38);
    if (edgeRoot) {
        // The fourth argument was an uninitialized stack slot in the original.
        WaypointGraph_DestroyEdgeSet(
            edgeRoot,
            CSF_AT(int, self, 0x3C),
            reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0x34),
            0);
        int edgeNodes = CSF_AT(int, self, 0x38);
        if (edgeNodes) {
            Mem_FreeDebug(reinterpret_cast<void*>(edgeNodes));
        }
    }
    CSF_AT(int, self, 0x38) = 0;
    CSF_AT(int, self, 0x3C) = 0;
    CSF_AT(int, self, 0x40) = 0;

    int result = CSF_AT(int, self, 0x24);
    if (result) {
        result = Mem_Free(reinterpret_cast<void*>(result));
        CSF_AT(int, self, 0x24) = 0;
    }

    // Small-string optimisation: only heap-free the buffer when capacity >= 16.
    if (CSF_AT(uint32_t, self, 0x1C) >= 0x10u) {
        result = CSF_AT(int, self, 0x08);
        if (result) {
            result = Mem_FreeDebug(reinterpret_cast<void*>(result));
        }
    }

    CSF_AT(int, self, 0x18) = 0;
    CSF_AT(uint32_t, self, 0x1C) = 0x0F;
    CSF_AT(uint8_t, self, 0x08) = 0;
    return result;
}

HOOK(0x75D430, WaypointGraph_ClearStorage, "auto");
