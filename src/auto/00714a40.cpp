#include "globals.h"
#include "hook.h"

// ---------------------------------------------------------------------------
// 0x00714410 - __cdecl callee (rets 0, i.e. the caller cleans the stack).
// The wrapper below pushes five dwords before the call (add esp, 0x14), so the
// callee effectively consumes five stack arguments:
//   arg1 = a1      (pushed last)
//   arg2 = a2
//   arg3 = a3
//   arg4 = this    (incoming ecx value)
//   arg5 = a3      (pushed first)
// ---------------------------------------------------------------------------
CSF_FUNC(void*, __cdecl, ForwardedCall, 0x714410,
         (void* a1, void* a2, void* a3, void* a4, void* a5));

// ---------------------------------------------------------------------------
// 0x00714A40 - thin forwarder.  ecx carries a value which is passed on as an
// extra argument, hence the __thiscall-style __fastcall (ecx = self, unused
// edx); it has three DWORD stack arguments, so the callee-popped byte count of
// the original (ret 0xC) is reproduced exactly.
//   entry : ecx = self, stack args a1, a2, a3
//   pushes: a3, self, a3, a2, a1
//   call  : ForwardedCall(a1, a2, a3, self, a3)
// ---------------------------------------------------------------------------
static void* __fastcall ForwardTo714410(void* self, void* /*edx*/,
                                        void* a1, void* a2, void* a3)
{
    return ForwardedCall(a1, a2, a3, self, a3);
}

HOOK(0x714A40, ForwardTo714410, "auto");
