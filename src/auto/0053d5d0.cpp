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

// __cdecl, no arguments, returns the global scheduler handle (GetGlobalValue_8B0568).
CSF_FUNC(int, __cdecl, GetGlobalValue_8B0568, 0x4FDBB0, ());

// __thiscall method (this in ecx), returns the number of bound contexts (SchedulerBase::GetNumberOfBoundContexts).
CSF_FUNC(uint32_t, __fastcall, SchedulerBase_GetNumberOfBoundContexts, 0x4FE490, (int scheduler, void* /*edx*/));

// Register-argument helper (count in ecx, no stack args); returns a page/context count.
CSF_FUNC(uint32_t, __fastcall, GetPageCountForContexts, 0x681F30, (uint32_t contextCount, void* /*edx*/));

// Returns true when the given index equals the last page index for the currently bound contexts.
// Original: if (GetPageCount(n) != 0) and index != ((GetPageCount(n2)+2)/3 - 1) -> false; otherwise true.
static bool __stdcall CMenuMissionInfo_IsIndexOnLastPage(int index)
{
    int scheduler = GetGlobalValue_8B0568();
    uint32_t boundContexts = SchedulerBase_GetNumberOfBoundContexts(scheduler, nullptr);

    if (GetPageCountForContexts(boundContexts, nullptr) != 0)
    {
        int schedulerAgain = GetGlobalValue_8B0568();
        uint32_t boundContextsAgain = SchedulerBase_GetNumberOfBoundContexts(schedulerAgain, nullptr);
        uint32_t lastPageIndex = (GetPageCountForContexts(boundContextsAgain, nullptr) + 2u) / 3u - 1u;
        if (static_cast<uint32_t>(index) != lastPageIndex)
            return false;
    }
    return true;
}

HOOK(0x53D5D0, CMenuMissionInfo_IsIndexOnLastPage, "auto");
