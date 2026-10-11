#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original callee bindings (addresses from the packet).
CSF_FUNC(uint32_t*, __fastcall, CheatsApplyFlagToTarget, 0x00428680, (void* self, void* edx, int a2, int a3));
CSF_FUNC(char, __fastcall, CheatsRefreshMenuNode, 0x0042CA40, (void* self, void* edx));

// 0x42CC30: __thiscall, one stack arg (a2, unused), ret 4.
// Applies the flag op to the handle stored in field_0010, then refreshes the
// parent node (this - 4) and returns its result.
static char __fastcall S_0042cc30_ApplyFlagThenRefresh(S_0042cc30* self, void* /*edx*/, int /*a2*/)
{
    CheatsApplyFlagToTarget(reinterpret_cast<void*>(self->field_0010), nullptr, 1, 0);
    return CheatsRefreshMenuNode(reinterpret_cast<uint8_t*>(self) - 4, nullptr);
}

HOOK(0x42CC30, S_0042cc30_ApplyFlagThenRefresh, "auto");
