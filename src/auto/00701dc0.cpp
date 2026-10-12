#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callee at 0x00701D40: __thiscall-style (this in ecx); a2 is passed on the stack, ret 4.
CSF_FUNC(char, __fastcall, sub_701D40, 0x00701D40, (void* self, void* edx, void* a2));

// Original: return sub_701D40(this, a2) == 0; (returns 1 when the callee returns 0)
static bool __fastcall GfxObjectInstances_CheckProjectionChange(S_00701d40* self, void* /*edx*/, int* a2)
{
    return sub_701D40(self, nullptr, a2) == 0;
}

HOOK(0x00701DC0, GfxObjectInstances_CheckProjectionChange, "auto");
