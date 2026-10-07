#include "globals.h"
#include "hook.h"

// ---------------------------------------------------------------------------
// 0x0072EFC0 - __cdecl callee (rets 0, i.e. the caller cleans the stack).
// The wrapper below pushes five dwords before the call (add esp, 0x14), so the
// callee effectively consumes five stack arguments:
//   arg1 = a1      (pushed last)
//   arg2 = a2
//   arg3 = a3
//   arg4 = self    (incoming ecx value)
//   arg5 = a3      (pushed first / a second time)
// ---------------------------------------------------------------------------
CSF_FUNC(int, __cdecl, ForwardedCall_72EFC0, 0x72EFC0,
         (void* a1, void* a2, void* a3, void* a4, void* a5));

// ---------------------------------------------------------------------------
// 0x0072F120 - thin forwarder.  ecx carries a value which is passed on as an
// extra argument, hence the __thiscall-style __fastcall (ecx = self, unused
// edx); it has three DWORD stack arguments, so the callee-popped byte count of
// the original (ret 0xC) is reproduced exactly.
//   entry : ecx = self, stack args a1, a2, a3
//   pushes: a3, self, a3, a2, a1
//   call  : ForwardedCall_72EFC0(a1, a2, a3, self, a3)
// ---------------------------------------------------------------------------
static int __fastcall ForwardTo72EFC0(void* self, void* /*edx*/,
                                      void* a1, void* a2, void* a3)
{
    return ForwardedCall_72EFC0(a1, a2, a3, self, a3);
}

HOOK(0x72F120, ForwardTo72EFC0, "auto");
