#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callees, bound by address.
CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(void, __fastcall, GlobalSetValue_5231D0, 0x005231D0, (int self, void* edx, int value));
CSF_FUNC(void, __cdecl, Nullsub_506AC0, 0x00506AC0, ());
CSF_FUNC(int, __fastcall, CMenuSaveGame_RefreshButtonsState, 0x00544880, (CMenuSaveGame* self, void* edx));
CSF_FUNC(void, __fastcall, CMenuSaveGame_RefreshListboxIfProxyReady, 0x00544A50, (CMenuSaveGame* self, void* edx));
CSF_FUNC(void, __fastcall, MenuSaveGame_RefreshLayoutForScheduler, 0x00544B90, (CMenuSaveGame* self, void* edx));
CSF_FUNC(void*, __fastcall, CMenuSaveGame_BeginNewSaveFlow, 0x005458D0, (CMenuSaveGame* self, void* edx));

// Stores the float argument in field_004c, pushes it to the global value, then
// dispatches on field_0050 (0 = reset listbox, 1 = begin new save, 5 = refresh
// listbox if proxy ready). Always finishes by refreshing the buttons.
static int __fastcall CMenuSaveGame_SetProgressAndDispatchMode(CMenuSaveGame* self, void* /*edx*/, int progressBits)
{
    // Store the stack argument reinterpreted as a float.
    *reinterpret_cast<float*>(&self->field_004c) = *reinterpret_cast<float*>(&progressBits);

    // The original keeps the argument on the stack across the cdecl GetGlobalValue_521BB0
    // call and passes the result in ecx, so the callee receives it as its stack arg.
    int globalValue = GetGlobalValue_521BB0();
    GlobalSetValue_5231D0(globalValue, nullptr, progressBits);
    Nullsub_506AC0();

    // Single read of the mode; the original does not reload it after calls.
    uint32_t dispatchMode = self->field_0050;
    if (dispatchMode == 0)
    {
        MenuSaveGame_RefreshLayoutForScheduler(self, nullptr);
    }
    else if (dispatchMode == 1)
    {
        CMenuSaveGame_BeginNewSaveFlow(self, nullptr);
    }
    else if (dispatchMode == 5)
    {
        CMenuSaveGame_RefreshListboxIfProxyReady(self, nullptr);
    }
    return CMenuSaveGame_RefreshButtonsState(self, nullptr);
}

HOOK(0x00545BD0, CMenuSaveGame_SetProgressAndDispatchMode, "auto");
