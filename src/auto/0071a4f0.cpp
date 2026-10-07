#include "globals.h"
#include "hook.h"

// ---------------------------------------------------------------------------
// 0x0071A510 - __cdecl callee (rets 0, so the caller cleans the stack).
// The wrapper below pushes five dwords before the call (add esp, 0x14), so the
// callee effectively consumes five stack arguments:
//   arg1 = a2      (pushed last)
//   arg2 = a3
//   arg3 = a4
//   arg4 = this    (incoming ecx value)
//   arg5 = a4      (pushed first)
// ---------------------------------------------------------------------------
CSF_FUNC(void*, __cdecl, ForwardedCall, 0x71A510,
         (void* a1, void* a2, void* a3, void* a4, void* a5));

// ---------------------------------------------------------------------------
// 0x0071A4F0 - thin forwarder.  ecx carries a value which is passed on as an
// extra argument, hence the __thiscall-style __fastcall (ecx = self, unused
// edx); it has three DWORD stack arguments (a2, a3, a4), so the callee-popped
// byte count of the original (ret 0xC) is reproduced exactly.
//   entry : ecx = self, stack args a2, a3, a4
//   pushes: a4, self, a4, a3, a2
//   call  : ForwardedCall(a2, a3, a4, self, a4)
// ---------------------------------------------------------------------------
static int __fastcall ForwardTo71A510(void* self, void* /*edx*/,
                                      void* a2, void* a3, void* a4)
{
    return reinterpret_cast<int>(ForwardedCall(a2, a3, a4, self, a4));
}

HOOK(0x71A4F0, ForwardTo71A510, "auto");
