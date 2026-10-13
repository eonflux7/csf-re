#include "hook.h"
#include "globals.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, sub_405180, 0x00405180, (void* self, void* edx, float* a2, float* a3, char a4, char a5, int* a6));

// Original: __thiscall (this in ecx) with 3 stack args a2, a3, a4, ret 12.
// Pushes, in order: a4, 1, 1, a3, a2. The callee's edx is not set by the original,
// so it is passed as nullptr. Callee stack args: a2, a3, 1, 1, a4.
static char __fastcall S_00405180_TraceGroundWithState(S_00405180* self, void* /*edx*/, float* a2, float* a3, int* a4)
{
    // The original references S_00405180's vtable; no store is made here.
    (void)CSF_VTBL(0x00405180);
    return sub_405180(self, nullptr, a2, a3, 1, 1, a4);
}

HOOK(0x004053A0, S_00405180_TraceGroundWithState, "auto");
