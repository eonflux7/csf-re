#include "globals.h"
#include "hook.h"

// ---------------------------------------------------------------------------
// 0x00558E50 - __cdecl callee (rets 0, the caller cleans the stack).
// The wrapper pushes five dwords and removes them with `add esp, 0x14`:
//   arg1 = a1      (pushed last)
//   arg2 = a2
//   arg3 = a3
//   arg4 = self    (the incoming ecx value)
//   arg5 = a3      (pushed first, re-read from [ebp+0x10])
// ---------------------------------------------------------------------------
CSF_FUNC(int*, __cdecl, sub_558E50, 0x00558E50,
         (int* a1, int* a2, int* a3, int a4, int a5));

// ---------------------------------------------------------------------------
// 0x00559050 - thin forwarder.  The original is __thiscall with ret 0xC; the
// incoming ecx is forwarded as an extra stack argument, so it is written as
// __fastcall (ecx = self, unused edx) with three DWORD stack arguments, which
// gives the same callee-popped byte count of 0xC.
// The `this` class S_004482c0 has no declared fields and is not in
// types/auto_classes.h, so self is taken as a plain void*.
//   entry : ecx = self, stack args a1, a2, a3
//   pushes: a3, self, a3, a2, a1
//   call  : sub_558E50(a1, a2, a3, self, a3)
// The return value is the callee's eax, passed through unchanged.
// ---------------------------------------------------------------------------
static int* __fastcall MpRules_ForwardToSabotageHelper(void* self, void* /*edx*/,
                                                       int* a1, int* a2, int* a3)
{
    return sub_558E50(a1, a2, a3, reinterpret_cast<int>(self), reinterpret_cast<int>(a3));
}

HOOK(0x559050, MpRules_ForwardToSabotageHelper, "auto");
