#include <cstdint>

#include "globals.h"
#include "hook.h"

// ---- tail-jumped state handlers ------------------------------------------
// Each arm of the original dispatcher ends in a tail jump into one of these
// handlers, forwarding the object pointer in ecx and an extra live-in context
// value kept in edi.  They are bound as __fastcall so that ecx (self) lines up
// with the first argument and every other register (notably edi) is forwarded
// unchanged.
CSF_FUNC(void, __fastcall, CMenuMpSessions_RefreshOptionControls,     0x560960, (void* self, void* /*edx*/));
CSF_FUNC(int,  __fastcall, MenuMpSessions_HandleState1Or3,  0x560BD0, (void* self, void* /*edx*/));
CSF_FUNC(int,  __fastcall, CMenuMpSessions_SetControlsActive,  0x560F80, (void* self, void* /*edx*/));

// Dispatches on the session state word stored at +0x1E0 of the menu object:
//   0     -> state 0 handler
//   1, 3  -> state 1/3 handler
//   2, 4  -> state 2/4 handler
// anything else falls through to a bare ret.
static void __fastcall MenuMpSessions_DispatchState(void* self, void* /*edx*/)
{
    switch (CSF_AT(uint32_t, self, 0x1e0))
    {
        case 0:
            CMenuMpSessions_RefreshOptionControls(self, nullptr);
            break;
        case 1:
        case 3:
            MenuMpSessions_HandleState1Or3(self, nullptr);
            break;
        case 2:
        case 4:
            CMenuMpSessions_SetControlsActive(self, nullptr);
            break;
        default:
            break;
    }
}

HOOK(0x561AF0, MenuMpSessions_DispatchState, "auto");
