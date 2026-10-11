#include "globals.h"
#include "hook.h"

CSF_FUNC(bool, __stdcall, NetCogible_ForwardMatchTest, 0x0065E6C0, (int a1));
CSF_GLOBAL(int, g_NetCogibleStoredId, 0x008BC128);

// Returns true when a1 equals the stored id; otherwise delegates to the helper.
static bool __stdcall NetCogibleDdl_MatchesStoredIdOrDelegate(int a1) {
    if (g_NetCogibleStoredId == a1)
        return true;
    return NetCogible_ForwardMatchTest(a1);
}

HOOK(0x00742040, NetCogibleDdl_MatchesStoredIdOrDelegate, "auto");
