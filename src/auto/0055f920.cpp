#include <cstdint>
#include "globals.h"
#include "hook.h"

// Original __cdecl helper at 0x550CC0 (no arguments, returns int).
CSF_FUNC(int, __cdecl, GetGlobal8B0688, 0x550CC0, ());

// Original __thiscall helpers (this in ecx, no stack arguments).
CSF_FUNC(int, __fastcall, MenuMpOnline_HandleOnlineNavigation, 0x55EF30, (void* self, void* edx));
CSF_FUNC(int, __fastcall, MenuMpOnline_ProcessPendingNavigation, 0x55F010, (void* self, void* edx));

CSF_GLOBAL(int, g_MpNetworkType, 0x89A5F0);
CSF_GLOBAL(std::uint8_t, g_OnlineMatchFlag, 0x8B068C);

// __thiscall with one stack argument (a2), popped by the callee: ret 4.
static int __fastcall MenuMpOnline_RouteNavigation(void* self, void* /*edx*/, int a2) {
    (void)a2;

    int useOnlinePath;
    if (GetGlobal8B0688()) {
        GetGlobal8B0688();
        useOnlinePath = g_OnlineMatchFlag;
    } else {
        useOnlinePath = (g_MpNetworkType == 3);
    }

    if (useOnlinePath) {
        return MenuMpOnline_HandleOnlineNavigation(self, nullptr);
    }
    return MenuMpOnline_ProcessPendingNavigation(self, nullptr);
}

HOOK(0x55F920, MenuMpOnline_RouteNavigation, "auto");
