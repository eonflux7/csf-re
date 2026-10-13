#include "globals.h"
#include "hook.h"

// ---------------------------------------------------------------------------
// 0x00782930 - __cdecl callee (rets 0, the caller cleans the stack).
// The wrapper pushes five dwords and removes them with `add esp, 0x14`:
//   arg1 = a1      (pushed last)
//   arg2 = a2
//   arg3 = a3
//   arg4 = self    (the incoming ecx value)
//   arg5 = a3      (pushed first, re-read from [ebp+0x10])
// ---------------------------------------------------------------------------
CSF_FUNC(int, __cdecl, sub_782930, 0x00782930,
         (int a1, int a2, int a3, void* a4, int a5));

// ---------------------------------------------------------------------------
// 0x00782B60 - thin forwarder.  The original is __thiscall with ret 0xC; the
// incoming ecx is forwarded as an extra stack argument, so it is written as
// __fastcall (ecx = self, unused edx) with three DWORD stack arguments, which
// gives the same callee-popped byte count of 0xC.
//   entry : ecx = self, stack args a1, a2, a3
//   pushes: a3, self, a3, a2, a1
//   call  : sub_782930(a1, a2, a3, self, a3)
// ---------------------------------------------------------------------------
static int __fastcall ForwardWithSelfToSub782930(void* self, void* /*edx*/,
                                                 int a1, int a2, int a3)
{
    return sub_782930(a1, a2, a3, self, a3);
}

HOOK(0x782B60, ForwardWithSelfToSub782930, "auto");
