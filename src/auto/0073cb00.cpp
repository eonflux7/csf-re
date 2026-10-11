#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Bindings to original functions (by address).
CSF_FUNC(char, __cdecl, EnsureNetworkInitOnce, 0x007A0150, ());
CSF_FUNC(char, __cdecl, IsSessionAddressReady, 0x00668A50, (int address));
CSF_FUNC(unsigned int, __fastcall, C_0085839c_ReleaseSessionResources, 0x0073C850, (void* self, void* edx));

// Global flag cleared when the session attempt falls back to the failed state.
CSF_GLOBAL(uint8_t, g_StationDumpPendingFlag, 0x008BBF98);

// Tries to advance the session state machine. Returns 1 when the session became ready (state 6),
// otherwise 0. Original is __thiscall with one stack argument (ret 4).
static char __fastcall C_0085839c_TryAdvanceSession(C_0085839c* self, void* /*edx*/, int a2)
{
    int state = CSF_AT(int, self, 0x98);
    if (state != 6 && state != 0)
    {
        if (!CSF_AT(uint8_t, self, 0x14f))
        {
            EnsureNetworkInitOnce();
            CSF_AT(uint8_t, self, 0x14f) = 1;
        }

        if (IsSessionAddressReady(a2))
        {
            CSF_AT(uint8_t, self, 0xfc) = 0;
            CSF_AT(uint8_t, self, 0x104) = 0;
            CSF_AT(int, self, 0x98) = 6;
            return 1;
        }

        C_0085839c_ReleaseSessionResources(self, nullptr);
        CSF_AT(int, self, 0x98) = 2;
        g_StationDumpPendingFlag = 0;
    }
    return 0;
}

HOOK(0x0073CB00, C_0085839c_TryAdvanceSession, "auto");
