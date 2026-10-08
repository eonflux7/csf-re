#include "globals.h"
#include "hook.h"

// ---------------------------------------------------------------------------
// 0x00714980 - __cdecl callee (rets 0, i.e. the caller cleans the stack).
// The wrapper below pushes five dwords before the call (add esp, 0x14), so the
// callee effectively consumes five stack arguments:
//   arg1 = a1  (pushed last)
//   arg2 = a2
//   arg3 = a3
//   arg4 = ecx (incoming ecx value)
//   arg5 = a3  (pushed first)
// ---------------------------------------------------------------------------
CSF_FUNC(int, __cdecl, CopyConstructPointerRange_714980, 0x714980,
         (int a1, int a2, int a3, int a4, int a5));

// ---------------------------------------------------------------------------
// 0x00714D10 - thin forwarder.  ecx carries a value which is passed on as an
// extra argument, hence the __thiscall-style __fastcall (ecx = self, unused
// edx); it has three DWORD stack arguments, so the callee-popped byte count of
// the original (ret 0xC) is reproduced exactly.
//   entry : ecx = self, stack args a1, a2, a3
//   pushes: a3, self, a3, a2, a1
//   call  : CopyConstructPointerRange_714980(a1, a2, a3, self, a3)
// ---------------------------------------------------------------------------
static int __fastcall ForwardTo714980(void* self, void* /*edx*/,
                                      int a1, int a2, int a3)
{
    return CopyConstructPointerRange_714980(a1, a2, a3, static_cast<int>(reinterpret_cast<intptr_t>(self)), a3);
}

HOOK(0x714D10, ForwardTo714980, "auto");
