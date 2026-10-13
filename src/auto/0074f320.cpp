#include "globals.h"
#include "hook.h"

CSF_FUNC(bool, __stdcall, NetCogible_ForwardMatchTest, 0x0065E6C0, (int a1));
CSF_GLOBAL(uint32_t, g_DdlObjectClassId, 0x008BC180);

// Returns true when a1 equals the DDL object class id; otherwise delegates to the helper.
static bool __stdcall NetPlayerListDdl_MatchesObjectClassIdOrDelegate(int a1) {
    if (g_DdlObjectClassId == static_cast<uint32_t>(a1))
        return true;
    return NetCogible_ForwardMatchTest(a1);
}

HOOK(0x0074F320, NetPlayerListDdl_MatchesObjectClassIdOrDelegate, "auto");
