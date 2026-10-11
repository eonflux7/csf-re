#include <cstdint>
#include "globals.h"
#include "hook.h"

// Mem_Free (game heap): bound by address, cdecl, one arg.
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Clears the node list rooted at the header passed as `self` and frees its nodes.
// Original signature from the packet; name chosen here.
CSF_FUNC(char, __fastcall, ClearPointListNodes, 0x0075EDC0, (void* self, void* edx, void** Block));

// Resets the waypoint buffer pointer at +4, clears the list hanging off the owner
// object at +0x18, and re-initializes the list sentinel node. Returns the sentinel.
static uint32_t* __fastcall WaypointGraph_ResetBufferAndList(void* self, void* /*edx*/)
{
    uint8_t* base = static_cast<uint8_t*>(self);

    uint32_t bufferPtr = CSF_AT(uint32_t, base, 0x4);
    CSF_AT(uint32_t, base, 0x8) = 0;
    CSF_AT(uint32_t, base, 0xC) = 0;
    if (bufferPtr) {
        Mem_Free(reinterpret_cast<void*>(bufferPtr));
        CSF_AT(uint32_t, base, 0x4) = 0;
    }

    // Owner object stored at +0x18; its +4 field is the list block handed to the clear routine.
    uint8_t* ownerObj = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, base, 0x18));
    uint32_t listBlock = CSF_AT(uint32_t, ownerObj, 0x4);
    ClearPointListNodes(base + 0x14, nullptr, reinterpret_cast<void**>(listBlock));

    // Re-initialize the sentinel node at +0x18.
    uint32_t sentinel = CSF_AT(uint32_t, base, 0x18);
    CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(sentinel), 0x4) = sentinel;

    sentinel = CSF_AT(uint32_t, base, 0x18);
    CSF_AT(uint32_t, base, 0x1C) = 0;
    CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(sentinel), 0x0) = sentinel;

    uint8_t* headerNode = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, base, 0x18));
    CSF_AT(uint32_t, headerNode, 0x8) = reinterpret_cast<uint32_t>(headerNode);
    return reinterpret_cast<uint32_t*>(headerNode);
}

HOOK(0x0075F160, WaypointGraph_ResetBufferAndList, "auto");
