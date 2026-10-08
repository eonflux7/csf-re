#include "globals.h"
#include "hook.h"

// ---------------------------------------------------------------------------
// 0x00433F10 - __cdecl callee (rets 0, i.e. the caller cleans the stack).
// The wrapper below pushes five dwords before the call and removes them with
// `add esp, 0x14`, so the callee effectively consumes five stack arguments:
//   arg1 = a1          (pushed last)
//   arg2 = a2
//   arg3 = a3
//   arg4 = self        (the incoming ecx value)
//   arg5 = a3          (pushed first, re-read from [ebp+0x10])
// ---------------------------------------------------------------------------
CSF_FUNC(int, __cdecl, CopyStringRange_433F10, 0x433F10,
         (void* a1, void* a2, int a3, void* a4, int a5));

// ---------------------------------------------------------------------------
// 0x00434400 - thin forwarder.  ecx carries a value which is forwarded as an
// extra stack argument, hence the __thiscall-style __fastcall (ecx = self,
// unused edx).  It has three DWORD stack arguments, reproducing the original's
// callee-popped byte count of 0xC.
//   entry : ecx = self, stack args a1, a2, a3
//   pushes: a3, self, a3, a2, a1
//   call  : CopyStringRange_433F10(a1, a2, a3, self, a3)
// ---------------------------------------------------------------------------
static int __fastcall ForwardStringPairTo433F10(void* self, void* /*edx*/,
                                                void* a1, void* a2, int a3)
{
    return CopyStringRange_433F10(a1, a2, a3, self, a3);
}

HOOK(0x434400, ForwardStringPairTo433F10, "auto");
