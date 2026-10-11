#include "globals.h"
#include "hook.h"

CSF_FUNC(void, __fastcall, Object_ApplyFloatParam, 0x00405BF0, (void* self, void* edx, float a2));

// Tail-jumps to 0x405BF0 with ecx/edx and the float argument unchanged.
// The callee pops the 4-byte float (ret 4), so this wrapper pops 4 too.
static void __fastcall C_0082d510_ForwardFloatToTarget(void* self, void* edx, float a2)
{
    Object_ApplyFloatParam(self, edx, a2);
}

HOOK(0x00405400, C_0082d510_ForwardFloatToTarget, "auto");
