#include "globals.h"
#include "hook.h"

// ---------------------------------------------------------------------------
// 0x00730850 - __cdecl callee (rets 0, i.e. caller cleans the stack).
// The wrapper below pushes five dwords before the call (add esp, 0x14), so the
// callee effectively receives five stack arguments:
//   arg1 = a1   (pushed last)
//   arg2 = a2
//   arg3 = a3
//   arg4 = self (incoming ecx value)
//   arg5 = a3   (pushed first)
// ---------------------------------------------------------------------------
CSF_FUNC(void*, __cdecl, ForwardedCall, 0x730850,
         (void* a1, void* a2, void* a3, void* a4, void* a5));

// ---------------------------------------------------------------------------
// 0x007309B0 - thin forwarder.  ecx carries a value that is passed on as an
// extra argument, hence the __thiscall-style __fastcall (ecx = self, unused
// edx).  It has three DWORD stack arguments, so the original's callee-popped
// byte count (ret 0xC) is reproduced exactly.
//   entry : ecx = self, stack args a1, a2, a3
//   pushes: a3, self, a3, a2, a1
//   call  : ForwardedCall(a1, a2, a3, self, a3)
// ---------------------------------------------------------------------------
static void* __fastcall ForwardTo730850(void* self, void* /*edx*/,
                                        void* a1, void* a2, void* a3)
{
    return ForwardedCall(a1, a2, a3, self, a3);
}

HOOK(0x7309B0, ForwardTo730850, "auto");
