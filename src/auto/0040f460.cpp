#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original: 0x004066D0, __fastcall-style callee with two stack args (char a2, int a3), ret 8.
CSF_FUNC(char, __fastcall, SetStateAndDispatchToTarget, 0x004066D0, (S_004054a0* self, void* /*edx*/, char a2, int a3));

// Original: 0x0040F150, __thiscall-style callee with no stack args, ret 0.
CSF_FUNC(char, __fastcall, sub_40F150, 0x0040F150, (void* self, void* /*edx*/));

// Forwards an input event (a2, a3) to the object held in field_001c, then runs the
// post-forward step on this object and returns its result.
static char __fastcall C_0082ddf8_ForwardInputEvent(C_0082ddf8* self, void* /*edx*/, char a2, int a3)
{
    SetStateAndDispatchToTarget(reinterpret_cast<S_004054a0*>(self->field_001c), nullptr, a2, a3);
    return sub_40F150(self, nullptr);
}

HOOK(0x0040F460, C_0082ddf8_ForwardInputEvent, "auto");
