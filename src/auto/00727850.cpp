#include "globals.h"
#include "hook.h"

// ---------------------------------------------------------------------------
// 0x00727450 - __cdecl callee (rets 0, i.e. the caller cleans the stack,
// `add esp, 0x14` below confirms five pushed dwords).
// Argument layout the wrapper establishes (top of stack first):
//   arg1 = a1
//   arg2 = a2
//   arg3 = a3
//   arg4 = this  (incoming ecx value)
//   arg5 = a3    (pushed first, i.e. deepest)
// ---------------------------------------------------------------------------
CSF_FUNC(int, __cdecl, CopyConstructRecordRange_727450, 0x727450,
         (void* a1, void* a2, void* a3, void* a4, void* a5));

// ---------------------------------------------------------------------------
// 0x00727850 - thin forwarder.  The incoming ecx value is forwarded as an extra
// argument, hence the __thiscall-style __fastcall (ecx = self, edx unused).  It
// takes three DWORD stack arguments, so the original's `ret 0xC` callee-popped
// byte count is reproduced exactly.
//   entry : ecx = self, stack args a1, a2, a3
//   pushes: a3, self, a3, a2, a1
//   call  : CopyConstructRecordRange_727450(a1, a2, a3, self, a3)
// ---------------------------------------------------------------------------
static int __fastcall ForwardTo727450(void* self, void* /*edx*/,
                                      void* a1, void* a2, void* a3)
{
    return CopyConstructRecordRange_727450(a1, a2, a3, self, a3);
}

HOOK(0x727850, ForwardTo727450, "auto");
