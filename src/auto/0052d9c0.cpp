#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Original callees, bound by address.
CSF_FUNC(void*, __cdecl, GetGlobalState, 0x0050D190, ());
CSF_FUNC(int, __fastcall, SchedulerBase_GetSchedulerProxy, 0x00734A10, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CMenuScenarioSelect_RebuildForGlobalMode, 0x0052D940, (CMenuScenarioSelect* self, void* edx));

// Stores the mode into the global state (+0x84), runs the rebuild on the scenario select screen,
// then clears the screen's pending flag (+0x38). Returns the rebuild's result.
static int CMenuScenarioSelect_ApplyGlobalModeAndRebuild(CMenuScenarioSelect* self, uint32_t mode)
{
    void* globalState = GetGlobalState();
    CSF_AT(uint32_t, globalState, 0x84) = mode;
    int rebuildResult = CMenuScenarioSelect_RebuildForGlobalMode(self, nullptr);
    self->field_0038 = 0;
    return rebuildResult;
}

static int __fastcall CMenuScenarioSelect_MapProxyCodeToGlobalMode(CMenuScenarioSelect* self, void* /*edx*/)
{
    int proxyCode = SchedulerBase_GetSchedulerProxy(GetGlobalState(), nullptr);

    if (proxyCode > 0x100) {
        if (proxyCode == 0x200) {
            return CMenuScenarioSelect_ApplyGlobalModeAndRebuild(self, 3);
        }
        if (proxyCode == 0x800) {
            self->field_0038 = 0;
        }
        return proxyCode;
    }

    if (proxyCode == 0x100) {
        return CMenuScenarioSelect_ApplyGlobalModeAndRebuild(self, 2);
    }

    // Original performs two successive subtractions of 0x40 on the code.
    // Unmatched codes return the twice-decremented value (code - 0x80), which is preserved here.
    uint32_t afterFirstSubtract = static_cast<uint32_t>(proxyCode) - 0x40u;
    if (afterFirstSubtract == 0) {
        return CMenuScenarioSelect_ApplyGlobalModeAndRebuild(self, 0);
    }
    uint32_t afterSecondSubtract = afterFirstSubtract - 0x40u;
    if (afterSecondSubtract == 0) {
        return CMenuScenarioSelect_ApplyGlobalModeAndRebuild(self, 1);
    }
    return static_cast<int>(afterSecondSubtract);
}

HOOK(0x0052D9C0, CMenuScenarioSelect_MapProxyCodeToGlobalMode, "auto");
