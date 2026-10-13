#include <cstdint>
#include <cstring>
#include "globals.h"

#include "hook.h"
#include "types/auto_classes.h"

// Original 0x004DCE10: __fastcall-style thunk, ecx = this, one stack arg.
CSF_FUNC(int, __fastcall, sub_4DCE10, 0x004DCE10, (void* self, void* edx, uint32_t* a2));

// Looks up (or reserves) the waypoint assessment slot for a2 and, if one is
// returned, copies the 0x50-byte assessment record from a3 into it.
// Returns the slot pointer (or NULL when the lookup fails).
static void* __fastcall StaticAi_StoreWaypointAssessment(S_004dc8f0* self, void* /*edx*/, std::uint32_t* a2, const void* a3)
{
    void* result = reinterpret_cast<void*>(sub_4DCE10(self, nullptr, reinterpret_cast<uint32_t*>(a2)));
    if (result)
        std::memcpy(result, a3, 0x50);
    return result;
}

HOOK(0x004DD110, StaticAi_StoreWaypointAssessment, "auto");
