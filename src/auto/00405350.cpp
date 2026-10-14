#include "hook.h"
#include "globals.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, sub_405180, 0x00405180, (void* self, void* edx, float* a2, float* a3, char a4, char a5, int* a6));

// Original: __thiscall with 3 stack args, ret 12. Stack pushes are, in order:
// [ebp+0x10], 0, 1, [ebp+0xc], [ebp+8]. Callee gets ecx=this (unchanged), edx=[ebp+0xc] (a3),
// and its stack args are a2=[ebp+8], a3=[ebp+0xc], a4=1, a5=0, a6=[ebp+0x10].
static char __fastcall S_00405350_ForwardWithDefaultFlags(S_00405180* self, void* /*edx*/, float* a2, float* a3, int* a4)
{
    // The original never reads its fourth stack argument ([ebp+0x14]); a4 is [ebp+0x10], passed as a6.
    // The original references S_00405180's vtable; no store is made here.
    (void)CSF_VTBL(0x00405180);
    return sub_405180(self, reinterpret_cast<void*>(a3), a2, a3, 1, 0, a4);
}

HOOK(0x00405350, S_00405350_ForwardWithDefaultFlags, "auto");
