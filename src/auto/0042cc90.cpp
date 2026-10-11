#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original callee bindings (addresses from the packet).
CSF_FUNC(uint32_t*, __fastcall, CheatsApplyFlagToTarget, 0x004286E0, (void* self, void* edx, int a2, int a3));
CSF_FUNC(char, __fastcall, CheatsRefreshMenuNode, 0x0042CA40, (void* self, void* edx));

// 0x42CC90: __thiscall, one stack arg (a2, unused), ret 4.
// Applies the flag pair (1, 2) to the handle stored at +0x10, then refreshes
// the parent node (this - 4) and returns its result.
static char __fastcall S_0042cc90_ApplyFlagPairThenRefresh(void* self, void* /*edx*/, int /*a2*/)
{
    uint8_t* base = static_cast<uint8_t*>(self);
    CheatsApplyFlagToTarget(reinterpret_cast<void*>(CSF_AT(uint32_t, base, 0x10)), nullptr, 1, 2);
    return CheatsRefreshMenuNode(base - 4, nullptr);
}

HOOK(0x42CC90, S_0042cc90_ApplyFlagPairThenRefresh, "auto");
