#include "globals.h"
#include "hook.h"

// ---------------------------------------------------------------------------
// 0x004526F0 - __cdecl callee (rets 0, the caller cleans the stack).
// The wrapper pushes five dwords and removes them with `add esp, 0x14`:
//   arg1 = a1      (pushed last)
//   arg2 = a2
//   arg3 = a3
//   arg4 = self    (the incoming ecx value, pushed as a plain value)
//   arg5 = a3      (pushed first, re-read from [ebp+0x10])
// ---------------------------------------------------------------------------
CSF_FUNC(int, __cdecl, RangeHelper_4526F0, 0x4526F0,
         (int a1, int a2, int a3, int a4, int a5));

// ---------------------------------------------------------------------------
// 0x004525C0 - thin forwarder.  __thiscall with ret 0xC: ecx = this, three
// DWORD stack arguments.  Written as __fastcall (ecx = self, unused edx),
// which gives the same callee-popped byte count of 0xC.
// The class S_004482c0 has no declared struct in types/auto_classes.h, so
// `this` is taken as a plain void pointer.
//   entry : ecx = self, stack args a1, a2, a3
//   pushes: a3, self, a3, a2, a1
//   call  : RangeHelper_4526F0(a1, a2, a3, self, a3)
// ---------------------------------------------------------------------------
static int __fastcall ForwardToRangeHelper_4526F0(void* self, void* /*edx*/,
                                                  int a1, int a2, int a3)
{
    return RangeHelper_4526F0(a1, a2, a3, reinterpret_cast<int>(self), a3);
}

HOOK(0x4525C0, ForwardToRangeHelper_4526F0, "auto");
