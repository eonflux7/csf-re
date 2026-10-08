#include "globals.h"
#include "hook.h"

// ---------------------------------------------------------------------------
// 0x005470D0 - __cdecl callee (rets 0, so the caller cleans the stack).
// The wrapper below pushes five dwords before the call (add esp, 0x14), so the
// callee effectively consumes five stack arguments:
//   arg1 = a1      (pushed last)
//   arg2 = a2
//   arg3 = a3
//   arg4 = self    (incoming ecx value)
//   arg5 = a3      (pushed first, same value as arg3)
// ---------------------------------------------------------------------------
CSF_FUNC(int, __cdecl, MenuExtras_ForwardTarget, 0x5470D0,
         (const void* a1, const void* a2, int a3, void* self, int a5));

// ---------------------------------------------------------------------------
// 0x00547700 - thin forwarder (32 bytes).  ecx carries a value that is passed
// on as an extra argument, hence the __thiscall-style __fastcall (ecx = self,
// unused edx).  It has three DWORD stack arguments, so the callee-popped byte
// count of the original (ret 0xC) is reproduced exactly.
//   entry : ecx = self, stack args a1, a2, a3
//   pushes: a3, self, a3, a2, a1
//   call  : MenuExtras_ForwardTarget(a1, a2, a3, self, a3)
// ---------------------------------------------------------------------------
static int __fastcall MenuExtras_ForwardToTarget(void* self, void* /*edx*/,
                                                 const void* a1, const void* a2,
                                                 int a3)
{
    return MenuExtras_ForwardTarget(a1, a2, a3, self, a3);
}

HOOK(0x547700, MenuExtras_ForwardToTarget, "auto");
