#include <cstdint>
#include "globals.h"
#include "hook.h"

// Initializes a 5-dword search-order record from a two-dword point (a2[0], a2[1]).
// Fields 0x08..0x10 are zeroed; the first two dwords are copied from a2.
static uint32_t* __fastcall AiSearchOrders_InitializePointFromWaypoint(void* self, void* /*edx*/, const uint32_t* a2)
{
    CSF_AT(uint32_t, self, 0x00) = 0xFFFFFFFF;
    CSF_AT(uint32_t, self, 0x04) = 0xFFFFFFFF;
    CSF_AT(uint32_t, self, 0x08) = 0;
    CSF_AT(uint32_t, self, 0x0C) = 0;
    CSF_AT(uint32_t, self, 0x10) = 0;
    CSF_AT(uint32_t, self, 0x00) = a2[0];
    CSF_AT(uint32_t, self, 0x04) = a2[1];
    return static_cast<uint32_t*>(self);
}

HOOK(0x4A9DE0, AiSearchOrders_InitializePointFromWaypoint, "auto");
