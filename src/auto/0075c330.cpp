#include <cstdint>
#include "globals.h"
#include "hook.h"

// __thiscall (this = ecx, one stack arg), ret 4. The second argument is passed
// through untouched, so it is typed as uint32_t*.
CSF_FUNC(uint32_t*, __fastcall, WaypointGraph_ProcessEntry, 0x0075C270, (void* self, void* edx, uint32_t* a2));

// Forwards a2 to WaypointGraph_ProcessEntry (with a1 as the this pointer) when a1
// is non-null; otherwise nothing is done. The original returned an uninitialized
// eax in that case; this returns nullptr instead.
static uint32_t* __stdcall Waypoint_ForwardToNodeIfNonNull(uint32_t* a1, uint32_t* a2)
{
    if (a1)
        return WaypointGraph_ProcessEntry(reinterpret_cast<void*>(a1), nullptr, a2);
    return nullptr;
}

HOOK(0x0075C330, Waypoint_ForwardToNodeIfNonNull, "auto");
