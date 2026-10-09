#include "globals.h"
#include "hook.h"

// ---------------------------------------------------------------------------
// 0x0046C8B0 - __cdecl callee (rets 0, the caller cleans the stack).
// The wrapper pushes five dwords and removes them with `add esp, 0x14`:
//   arg1 = a1      (pushed last)
//   arg2 = a2
//   arg3 = a3
//   arg4 = self    (the incoming ecx value)
//   arg5 = a3      (pushed first, re-read from [ebp+0x10])
// ---------------------------------------------------------------------------
CSF_FUNC(int, __cdecl, sub_46C8B0, 0x0046C8B0,
         (void* a1, void* a2, int a3, int a4, int a5));

// ---------------------------------------------------------------------------
// 0x0046D9D0 - thin forwarder.  The original is __thiscall with ret 0xC.
// It forwards the incoming ecx (self) as an extra stack argument, so it is
// written as __fastcall (ecx = self, unused edx) with three DWORD stack
// arguments, which gives the same callee-popped byte count of 0xC.
//   entry : ecx = self, stack args a1, a2, a3
//   pushes: a3, self, a3, a2, a1
//   call  : sub_46C8B0(a1, a2, a3, self, a3)
// ---------------------------------------------------------------------------
static int __fastcall MissionScript_ForwardToParseEntry(void* self, void* /*edx*/,
                                                        void* a1, void* a2, int a3)
{
    return sub_46C8B0(a1, a2, a3, reinterpret_cast<int>(self), a3);
}

HOOK(0x46D9D0, MissionScript_ForwardToParseEntry, "auto");
