#include "globals.h"
#include "hook.h"

// ---------------------------------------------------------------------------
// 0x007310F0 - __cdecl callee (rets 0, i.e. the caller cleans the stack).
// The wrapper below pushes five dwords before the call (add esp, 0x14), so it
// consumes five stack arguments:
//   arg1 = a1            (pushed last)
//   arg2 = a2
//   arg3 = a3
//   arg4 = incoming ecx  (the object / context pointer, pushed as `push ecx`)
//   arg5 = a3            (pushed first, duplicate of arg3)
// ---------------------------------------------------------------------------
CSF_FUNC(int, __cdecl, ForwardedCall, 0x7310F0,
         (int a1, int a2, int a3, void* a4, int a5));

// ---------------------------------------------------------------------------
// 0x007312A0 - thin forwarder.  ecx carries a value (this / context) which is
// passed on as an extra argument, hence the __thiscall-style __fastcall
// (ecx = self, unused edx).  It has three DWORD stack arguments, so the
// callee-popped byte count of the original (ret 0xC) is reproduced exactly.
//   entry : ecx = self, stack args a1, a2, a3
//   pushes: a3, self, a3, a2, a1
//   call  : ForwardedCall(a1, a2, a3, self, a3)
// ---------------------------------------------------------------------------
static int __fastcall ForwardTo7310F0(void* self, void* /*edx*/,
                                      int a1, int a2, int a3)
{
    return ForwardedCall(a1, a2, a3, self, a3);
}

HOOK(0x7312A0, ForwardTo7310F0, "auto");
