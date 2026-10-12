#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(bool, __stdcall, NetPlayerDdl_ForwardMatchTest, 0x007A3070, (int a1));
CSF_GLOBAL(std::uint32_t, g_NetPlayerClassId, 0x008BC15C);

static bool __stdcall NetPlayerDdl_MatchesClassIdOrDelegate(int classId) {
    if (g_NetPlayerClassId == static_cast<std::uint32_t>(classId))
        return true;
    return NetPlayerDdl_ForwardMatchTest(classId);
}

HOOK(0x00744610, NetPlayerDdl_MatchesClassIdOrDelegate, "auto");
