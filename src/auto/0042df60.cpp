#include <cstdint>
#include "globals.h"
#include "hook.h"

// Callee at 0x4F3EA0. Its first argument arrives in ecx and is the sub-object pointer
// loaded from this+0x284. ebx and edi are register inputs the original never sets,
// so they are inherited from the caller and cannot be reproduced here.
CSF_FUNC(int, __fastcall, HudLayoutTextAndValue, 0x4F3EA0, (int subObject, void* edx, int a4, float a5, int a6));

// Original: __stdcall (ecx = this, args a1 and a2 on the stack, ret 8).
// Reads the sub-object pointer from self+0x284 and forwards it with a6 = -1.
static int __fastcall HudSinkTextAndValue(void* self, void* /*edx*/, int a1, float a2)
{
    int subObject = CSF_AT(int, self, 0x284);
    return HudLayoutTextAndValue(subObject, nullptr, a1, a2, -1);
}

HOOK(0x0042DF60, HudSinkTextAndValue, "auto");
