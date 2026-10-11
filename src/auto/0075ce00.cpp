#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, WaypointGraph_SearchFacing, 0x0075CB30, (void* self, void* edx, int a2, float* a3, uint32_t* a4));

static char __fastcall C_0083e0d8_SearchFacingTarget(C_0083e0d8* self, void* /*edx*/, float* a2, uint32_t* a3)
{
    return WaypointGraph_SearchFacing(self, reinterpret_cast<void*>(a2), -1, a2, a3);
}

HOOK(0x0075CE00, C_0083e0d8_SearchFacingTarget, "auto");
