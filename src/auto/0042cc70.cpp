#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original callee bindings (addresses from the packet).
CSF_FUNC(uint32_t*, __fastcall, CheatsApplyFlagToTarget, 0x00428680, (void* self, void* edx, int a2, int a3));
CSF_FUNC(char, __fastcall, CheatsRefreshMenuNode, 0x0042CA40, (void* self, void* edx));

// 0x42CC70: __thiscall, one stack arg (a2, unused), ret 4.
// Applies the flag op (1, 2) to the handle stored at +0x10 (this[4]), then
// refreshes the parent node (this - 4) and returns its result.
static char __fastcall S_0042cc70_ApplyFlagThenRefreshParent(uint8_t* self, void* /*edx*/, int /*a2*/)
{
    CheatsApplyFlagToTarget(reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x10)), nullptr, 1, 2);
    return CheatsRefreshMenuNode(self - 4, nullptr);
}

HOOK(0x42CC70, S_0042cc70_ApplyFlagThenRefreshParent, "auto");
