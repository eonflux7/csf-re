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

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, CallInnerObjectVirtualMethod110, 0x00492BC0, (void* self, void* edx));

CSF_GLOBAL(float, g_ReloadTimeThreshold, 0x00833430);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// Accumulates elapsed time into the timer at +0xAC. Once it crosses the threshold,
// resolves the handle at +0x40 and notifies that object. Then, if the flag at +0x69
// is set, invokes virtual slot 77 (+0x134) with (0, 0) and returns its result.
static char __fastcall CharActionFsm_AdvanceReloadTimer(void* self, void* /*edx*/, float deltaSeconds)
{
    if (CSF_AT(float, self, 0xAC) < g_ReloadTimeThreshold)
    {
        float updatedTimer = deltaSeconds + CSF_AT(float, self, 0xAC);
        CSF_AT(float, self, 0xAC) = updatedTimer;

        if (updatedTimer >= g_ReloadTimeThreshold)
        {
            int handleAddress = (int)((uint8_t*)self + 0x40);
            int resolvedObject = CHandleTable_Resolve(g_pCurrentScene, nullptr, handleAddress);
            if (resolvedObject)
                CallInnerObjectVirtualMethod110((void*)resolvedObject, nullptr);
        }
    }

    char pendingFlag = CSF_AT(char, self, 0x69);
    if (pendingFlag)
    {
        return ((char(__fastcall*)(void*, void*, int, int))CSF_AT(void**, self, 0)[77])(self, nullptr, 0, 0);
    }
    return pendingFlag;
}

HOOK(0x00483D90, CharActionFsm_AdvanceReloadTimer, "auto");
