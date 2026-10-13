#include "globals.h"
#include "hook.h"

CSF_FUNC(bool, __stdcall, NetCogible_ForwardMatchTest, 0x0065E6C0, (int a1));
CSF_GLOBAL(std::uint32_t, g_NetControlClassId, 0x008BC17C);

// Returns true when a1 equals the NetControl class id; otherwise delegates to the forward-match helper.
static bool __stdcall NetControlDdl_MatchesClassIdOrDelegate(int a1) {
    if (g_NetControlClassId == static_cast<std::uint32_t>(a1))
        return true;
    return NetCogible_ForwardMatchTest(a1);
}

HOOK(0x0074E670, NetControlDdl_MatchesClassIdOrDelegate, "auto");
