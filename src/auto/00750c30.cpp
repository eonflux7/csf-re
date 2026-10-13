#include "globals.h"
#include "hook.h"

CSF_FUNC(bool, __stdcall, NetPlayerDdl_ForwardMatchTest, 0x007A3070, (int a1));
CSF_GLOBAL(uint32_t, g_DdlObjectClassId, 0x008BC1CC);

// Returns true when a1 equals the stored DDL object class id; otherwise delegates to the helper.
static bool __stdcall NetChatDdl_MatchesStoredClassIdOrDelegate(int a1) {
    if (g_DdlObjectClassId == static_cast<uint32_t>(a1))
        return true;
    return NetPlayerDdl_ForwardMatchTest(a1);
}

HOOK(0x00750C30, NetChatDdl_MatchesStoredClassIdOrDelegate, "auto");
