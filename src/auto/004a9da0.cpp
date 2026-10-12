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

// __thiscall: ecx = this (5-dword search point), one stack arg (ret 4).
// Clears the record to {-1, -1, 0, 0, 0}, sets the sentinel pair to -1 again,
// then copies the three dwords of the source point into slots 2..4.
static uint32_t* __fastcall AiSearchOrders_InitializeSearchPoint(uint32_t* self, void* /*edx*/, const uint32_t* sourcePoint)
{
    CSF_AT(uint32_t, self, 0x00) = 0xFFFFFFFFu;
    CSF_AT(uint32_t, self, 0x04) = 0xFFFFFFFFu;
    CSF_AT(uint32_t, self, 0x08) = 0;
    CSF_AT(uint32_t, self, 0x0C) = 0;
    CSF_AT(uint32_t, self, 0x10) = 0;

    CSF_AT(uint32_t, self, 0x00) = 0xFFFFFFFFu;
    CSF_AT(uint32_t, self, 0x04) = 0xFFFFFFFFu;
    CSF_AT(uint32_t, self, 0x08) = sourcePoint[0];
    CSF_AT(uint32_t, self, 0x0C) = sourcePoint[1];
    CSF_AT(uint32_t, self, 0x10) = sourcePoint[2];

    return self;
}

HOOK(0x004A9DA0, AiSearchOrders_InitializeSearchPoint, "auto");
