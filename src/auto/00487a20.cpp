#include "globals.h"
#include "hook.h"

// ---------------------------------------------------------------------------
// 0x00484090 - __cdecl callee (rets 0, the caller cleans the stack).
// The wrapper pushes five dwords and removes them with `add esp, 0x14`, so the
// callee consumes five stack arguments:
//   arg1 = a1      (pushed last)
//   arg2 = a2
//   arg3 = a3
//   arg4 = self    (the incoming ecx value, pushed as a plain value)
//   arg5 = a3      (pushed first, re-read from [ebp+0x10])
// ---------------------------------------------------------------------------
CSF_FUNC(int, __cdecl, CharActionFsm_StateHelper_484090, 0x484090,
         (int a1, int a2, int a3, void* a4, int a5));

// ---------------------------------------------------------------------------
// 0x00487A20 - thin forwarder.  The original is __thiscall-style with ret 0xC.
// It forwards the incoming ecx as an extra stack argument, so it is written
// as __fastcall (ecx = self, unused edx) with three DWORD stack arguments,
// which gives the same callee-popped byte count of 0xC.
//   entry : ecx = self, stack args a1, a2, a3
//   pushes: a3, self, a3, a2, a1
//   call  : CharActionFsm_StateHelper_484090(a1, a2, a3, self, a3)
// ---------------------------------------------------------------------------
static int __fastcall CharActionFsm_ForwardToStateHelper(void* self, void* /*edx*/,
                                                         int a1, int a2, int a3)
{
    return CharActionFsm_StateHelper_484090(a1, a2, a3, self, a3);
}

HOOK(0x487A20, CharActionFsm_ForwardToStateHelper, "auto");
