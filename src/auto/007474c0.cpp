#include "globals.h"
#include "hook.h"

CSF_FUNC(bool, __stdcall, NetCogible_ForwardMatchTest, 0x0065E6C0, (int a1));
CSF_GLOBAL(std::uint32_t, g_NetProyectilClassId, 0x008BC160);

// Returns true when a1 equals the stored projectile class id; otherwise delegates to the helper.
static bool __stdcall NetProyectilDdl_MatchesClassIdOrDelegate(int a1) {
    if (g_NetProyectilClassId == static_cast<std::uint32_t>(a1))
        return true;
    return NetCogible_ForwardMatchTest(a1);
}

HOOK(0x007474C0, NetProyectilDdl_MatchesClassIdOrDelegate, "auto");
