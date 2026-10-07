#include "globals.h"
#include "hook.h"

// ---------------------------------------------------------------------------
// 0x0071A480 - __cdecl callee (rets 0, caller cleans the stack). The wrapper
// below pushes five dwords before the call (add esp, 0x14), so the callee
// effectively consumes five stack arguments:
//   arg1 = a1     (pushed last)
//   arg2 = a2
//   arg3 = a3
//   arg4 = self   (incoming ecx value)
//   arg5 = a3     (pushed first)
// ---------------------------------------------------------------------------
CSF_FUNC(int, __cdecl, ForwardedCall, 0x71A480,
         (int a1, int a2, int a3, int a4, int a5));

// ---------------------------------------------------------------------------
// 0x0071AF10 - thin forwarder. ecx carries a value passed on as an extra
// argument, hence the __thiscall-style __fastcall (ecx = self, unused edx).
// It has three DWORD stack arguments, so the original's ret 0xC is
// reproduced exactly.
//   entry : ecx = self, stack args a1, a2, a3
//   pushes: a3, self, a3, a2, a1
//   call  : ForwardedCall(a1, a2, a3, self, a3)
// ---------------------------------------------------------------------------
static int __fastcall ForwardTo71A480(void* self, void* /*edx*/,
                                      int a1, int a2, int a3)
{
    return ForwardedCall(a1, a2, a3, (int)self, a3);
}

HOOK(0x71AF10, ForwardTo71A480, "auto");
