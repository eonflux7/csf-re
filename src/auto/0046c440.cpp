#include "globals.h"
#include "hook.h"

// ---------------------------------------------------------------------------
// 0x0046C0C0 - __cdecl callee (rets 0, the caller cleans the stack).
// The wrapper pushes five dwords and removes them with `add esp, 0x14`, so the
// callee effectively consumes five stack arguments:
//   arg1 = a1      (pushed last)
//   arg2 = a2
//   arg3 = a3
//   arg4 = self    (the incoming ecx value, pushed as a plain value)
//   arg5 = a3      (pushed first, re-read from [ebp+0x10])
// ---------------------------------------------------------------------------
CSF_FUNC(int, __cdecl, MissionScript_ParseEntryHelper_46C0C0, 0x46C0C0,
         (int a1, int a2, int a3, void* a4, int a5));

// ---------------------------------------------------------------------------
// 0x0046C440 - thin forwarder.  The original is __thiscall-style (ecx = self)
// with three DWORD stack arguments and ret 0xC.  It forwards the incoming ecx
// as an extra stack argument, so it is written as __fastcall (ecx = self,
// unused edx) with three DWORD stack arguments, which gives the same
// callee-popped byte count of 0xC.
//   entry : ecx = self, stack args a1, a2, a3
//   pushes: a3, self, a3, a2, a1
//   call  : MissionScript_ParseEntryHelper_46C0C0(a1, a2, a3, self, a3)
// ---------------------------------------------------------------------------
static int __fastcall MissionScript_ForwardToParseEntryHelper(void* self, void* /*edx*/,
                                                              int a1, int a2, int a3)
{
    return MissionScript_ParseEntryHelper_46C0C0(a1, a2, a3, self, a3);
}

HOOK(0x46C440, MissionScript_ForwardToParseEntryHelper, "auto");
