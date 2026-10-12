#include "globals.h"
#include "hook.h"

CSF_FUNC(bool, __stdcall, NetCogible_ForwardMatchTest, 0x0065E6C0, (int a1));
CSF_GLOBAL(std::uint32_t, g_NetGameClassId, 0x008BC178);

// Returns true when a1 equals the NetGame class id; otherwise delegates to the helper.
static bool __stdcall NetGameDdl_IsNetGameClassOrForwardMatch(int a1) {
    if (g_NetGameClassId == static_cast<std::uint32_t>(a1))
        return true;
    return NetCogible_ForwardMatchTest(a1);
}

HOOK(0x0074C500, NetGameDdl_IsNetGameClassOrForwardMatch, "auto");
