#include "globals.h"
#include "hook.h"

// ---------------------------------------------------------------------------
// 0x00716080 - __cdecl callee (rets 0, i.e. the caller cleans the stack).
// The wrapper below pushes five dwords before the call (add esp, 0x14), so the
// callee effectively consumes five stack arguments:
//   arg1 = a1      (pushed last, from [ebp+8])
//   arg2 = a2      (from [ebp+0xc])
//   arg3 = a3      (from [ebp+0x10])
//   arg4 = self    (the incoming ecx value)
//   arg5 = a3      (pushed first, re-read from [ebp+0x10])
// ---------------------------------------------------------------------------
CSF_FUNC(void*, __cdecl, CopyConstructRecordRange_716080, 0x716080,
         (void* a1, void* a2, void* a3, void* a4, void* a5));

// ---------------------------------------------------------------------------
// 0x00716B40 - thin forwarder.  ecx carries a value which is forwarded as the
// fourth argument, hence the __thiscall-style __fastcall (ecx = self, unused
// edx); it has three DWORD stack arguments, so the callee-popped byte count of
// the original (ret 0xC) is reproduced exactly.
//   entry : ecx = self, stack args a1, a2, a3
//   pushes: a3, self, a3, a2, a1
//   call  : CopyConstructRecordRange_716080(a1, a2, a3, self, a3)
// ---------------------------------------------------------------------------
static void* __fastcall ForwardTo716080(void* self, void* /*edx*/,
                                        void* a1, void* a2, void* a3)
{
    return CopyConstructRecordRange_716080(a1, a2, a3, self, a3);
}

HOOK(0x716B40, ForwardTo716080, "auto");
