#include "globals.h"
#include "hook.h"

// ---------------------------------------------------------------------------
// 0x0055AA90 - __cdecl helper (rets 0, the caller cleans the stack).
// The wrapper pushes five dwords and removes them with `add esp, 0x14`:
//   arg1 = a1      (pushed last)
//   arg2 = a2
//   arg3 = a3
//   arg4 = self    (the incoming ecx value)
//   arg5 = a3      (pushed first, re-read from [ebp+0x10])
// ---------------------------------------------------------------------------
CSF_FUNC(int, __cdecl, TeamRules_Helper55AA90, 0x0055AA90,
         (int a1, int a2, int a3, int a4, int a5));

// ---------------------------------------------------------------------------
// 0x0055ABA0 - thin forwarder.  The original is __thiscall with ret 0xC; the
// incoming ecx is forwarded as an extra stack argument, so it is written as
// __fastcall (ecx = self, unused edx) with three DWORD stack arguments, which
// gives the same callee-popped byte count of 0xC.
//   entry : ecx = self, stack args a1, a2, a3
//   pushes: a3, self, a3, a2, a1
//   call  : TeamRules_Helper55AA90(a1, a2, a3, self, a3)
// ---------------------------------------------------------------------------
static int __fastcall TeamRules_ForwardToHelper55AA90(void* self, void* /*edx*/,
                                                      int a1, int a2, int a3)
{
    return TeamRules_Helper55AA90(a1, a2, a3, reinterpret_cast<int>(self), a3);
}

HOOK(0x55ABA0, TeamRules_ForwardToHelper55AA90, "auto");
