#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Callees, bound by address.
CSF_FUNC(void, __cdecl, Nullsub_506AC0, 0x506AC0, ());
CSF_FUNC(int, __cdecl, GetGlobalState, 0x50D190, ());
CSF_FUNC(int, __cdecl, GetGlobalValue, 0x521BB0, ());
CSF_FUNC(void, __fastcall, GlobalValue_Apply, 0x5231D0, (void* self, void* edx, int value));
CSF_FUNC(int, __fastcall, SchedulerBase_GetSchedulerProxy, 0x734A10, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CMenuVideoOptions_GetMatchingQualityPreset, 0x549950, (CMenuVideoOptions* self, void* edx));
CSF_FUNC(int, __fastcall, CMenuVideoOptions_UpdateOptionsMenu, 0x549630, (CMenuVideoOptions* self, void* edx));

// Applies a new selection: stores it, syncs the quality preset, refreshes the global
// value, and, if a flag is set and the scheduler proxy reports 1, clears the flag and
// rebuilds the options menu. Returns the low byte of the final eax value.
static char __fastcall CMenuVideoOptions_ApplySelection(CMenuVideoOptions* self, void* /*edx*/, int selection)
{
    self->field_0078 = static_cast<uint32_t>(selection);

    const int matchingPreset = CMenuVideoOptions_GetMatchingQualityPreset(self, nullptr);
    if (matchingPreset != static_cast<int>(self->field_0030))
    {
        void* qualityObj = reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0058));
        self->field_0030 = static_cast<uint32_t>(matchingPreset);
        using SetPresetFn = void(__fastcall*)(void*, void*, int);
        ((SetPresetFn)CSF_AT(void**, qualityObj, 0)[170])(qualityObj, nullptr, matchingPreset);
    }

    const int selectionSnapshot = static_cast<int>(self->field_0078);
    const int globalValue = GetGlobalValue();
    GlobalValue_Apply(reinterpret_cast<void*>(static_cast<intptr_t>(globalValue)), nullptr, selectionSnapshot);
    Nullsub_506AC0();

    char result = static_cast<char>(self->field_007c);
    if (self->field_007c != 0)
    {
        const int proxy = SchedulerBase_GetSchedulerProxy(
            reinterpret_cast<void*>(static_cast<intptr_t>(GetGlobalState())), nullptr);
        const int remaining = proxy - 1;
        result = static_cast<char>(remaining);
        if (remaining == 0)
        {
            self->field_007c = 0;
            result = static_cast<char>(CMenuVideoOptions_UpdateOptionsMenu(self, nullptr));
        }
    }
    return result;
}

HOOK(0x00549E80, CMenuVideoOptions_ApplySelection, "auto");
