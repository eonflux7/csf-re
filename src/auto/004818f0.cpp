#include "globals.h"
#include "hook.h"

// ---------------------------------------------------------------------------
// 0x00480D00 - __cdecl callee (rets 0, the caller cleans the stack).
// The wrapper pushes five dwords and removes them with `add esp, 0x14`, so the
// callee effectively consumes five stack arguments:
//   arg1 = a1      (pushed last)
//   arg2 = a2
//   arg3 = a3
//   arg4 = self    (the incoming ecx value, pushed as a plain value)
//   arg5 = a3      (pushed first, re-read from [ebp+0x10])
// ---------------------------------------------------------------------------
CSF_FUNC(int, __cdecl, CharFsmDispatchHelper_480D00, 0x480D00,
         (int a1, int a2, int a3, void* a4, int a5));

// ---------------------------------------------------------------------------
// 0x004818F0 - thin forwarder.  The original is __thiscall with ret 0xC: ecx is
// self, and the three DWORD stack arguments are forwarded to the helper along
// with self and a repeated a3.  Written as __fastcall (ecx = self, unused edx)
// with three DWORD stack arguments, which gives the same callee-popped count.
//   entry : ecx = self, stack args a1, a2, a3
//   pushes: a3, self, a3, a2, a1
//   call  : CharFsmDispatchHelper_480D00(a1, a2, a3, self, a3)
// ---------------------------------------------------------------------------
static int __fastcall CharFsm_ForwardToDispatchHelper(void* self, void* /*edx*/,
                                                      int a1, int a2, int a3)
{
    return CharFsmDispatchHelper_480D00(a1, a2, a3, self, a3);
}

HOOK(0x4818F0, CharFsm_ForwardToDispatchHelper, "auto");
