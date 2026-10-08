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

// Original callees and data, bound by address.
CSF_GLOBAL(float, g_fIntroVideoDuration, 0x0082E118);

CSF_FUNC(int, __cdecl, GetGlobalDword_73C020, 0x0073C020, (void));
CSF_FUNC(int, __cdecl, GetGlobalState, 0x0050D190, (void));
CSF_FUNC(int, __fastcall, GlobalState_ResetPending, 0x0050E1E0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, MenuMpOnline_RefreshForState, 0x0055ECA0, (void* self, void* edx));
CSF_FUNC(uint8_t, __fastcall, GlobalFlag_IsSet, 0x007A7B50, (void* ecxArg, void* edx));

// Advances the intro timer by elapsed seconds; once it exceeds the intro video
// duration and the global flag is set, resets the global state, moves the menu
// to state 2 and refreshes it.
static void __fastcall MenuMpOnline_TickIntroTimer(void* self, void* /*edx*/, float elapsedSeconds)
{
    const float accumulated = elapsedSeconds + CSF_AT(float, self, 0x50);
    CSF_AT(float, self, 0x50) = accumulated;

    if (accumulated > g_fIntroVideoDuration) {
        const int globalDword = GetGlobalDword_73C020();
        if (GlobalFlag_IsSet(reinterpret_cast<void*>(static_cast<intptr_t>(globalDword)), nullptr)) {
            const int globalState = GetGlobalState();
            GlobalState_ResetPending(reinterpret_cast<void*>(static_cast<intptr_t>(globalState)), nullptr);
            CSF_AT(int, self, 0x18) = 2;
            MenuMpOnline_RefreshForState(self, nullptr);
        }
    }
}

HOOK(0x0055EE90, MenuMpOnline_TickIntroTimer, "auto");
