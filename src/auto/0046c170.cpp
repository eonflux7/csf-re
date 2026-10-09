#include "globals.h"
#include "hook.h"

// ---------------------------------------------------------------------------
// 0x0046BF40 - __cdecl callee (rets 0, the caller cleans the stack).
// The wrapper pushes five dwords and removes them with `add esp, 0x14`, so the
// callee effectively consumes five stack arguments:
//   arg1 = a1      (pushed last)
//   arg2 = a2
//   arg3 = a3
//   arg4 = self    (the incoming ecx value, pushed as a plain value)
//   arg5 = a3      (pushed first, re-read from [ebp+0x10])
// ---------------------------------------------------------------------------
CSF_FUNC(int, __cdecl, sub_46BF40, 0x0046BF40,
         (int a1, int a2, int a3, int a4, int a5));

// ---------------------------------------------------------------------------
// 0x0046C170 - thin forwarder.  The original is __thiscall with ret 0xC: it
// takes three DWORD stack arguments and forwards the incoming ecx (this) as an
// extra stack argument, so it is written as __fastcall (ecx = self, unused
// edx) with three DWORD stack arguments, giving the same callee-popped byte
// count of 0xC.
//   entry : ecx = self, stack args a1, a2, a3
//   pushes: a3, self, a3, a2, a1
//   call  : sub_46BF40(a1, a2, a3, self, a3)
// ---------------------------------------------------------------------------
static int __fastcall MissionScript_ForwardToParseHelper(void* self, void* /*edx*/,
                                                         int a1, int a2, int a3)
{
    return sub_46BF40(a1, a2, a3,
                      static_cast<int>(reinterpret_cast<std::intptr_t>(self)),
                      a3);
}

HOOK(0x46C170, MissionScript_ForwardToParseHelper, "auto");
