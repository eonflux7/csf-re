#include "globals.h"
#include "hook.h"

// ---------------------------------------------------------------------------
// 0x00549BA0 - __cdecl callee (rets 0, i.e. the caller cleans the stack).
// The wrapper below pushes five dwords before the call (add esp, 0x14), so the
// callee effectively consumes five stack arguments:
//   arg1 = a1    (pushed last)
//   arg2 = a2
//   arg3 = a3
//   arg4 = self  (incoming ecx value)
//   arg5 = a3    (pushed first)
// ---------------------------------------------------------------------------
CSF_FUNC(int, __cdecl, MenuVideoOptions_CopyElementRange, 0x549BA0,
         (int a1, int a2, int a3, void* self, int a5));

// ---------------------------------------------------------------------------
// 0x00549EE0 - thin forwarder belonging to the menu_video_options module.
// ecx carries a value which is passed on as an extra argument, hence the
// __thiscall-style __fastcall (ecx = self, unused edx); it has three DWORD
// stack arguments, so the callee-popped byte count of the original (ret 0xC)
// is reproduced exactly.
//   entry : ecx = self, stack args a1, a2, a3
//   pushes: a3, self, a3, a2, a1
//   call  : MenuVideoOptions_CopyElementRange(a1, a2, a3, self, a3)
// ---------------------------------------------------------------------------
static int __fastcall MenuVideoOptions_ForwardCall(void* self, void* /*edx*/,
                                                   int a1, int a2, int a3)
{
    return MenuVideoOptions_CopyElementRange(a1, a2, a3, self, a3);
}

HOOK(0x549EE0, MenuVideoOptions_ForwardCall, "auto");
