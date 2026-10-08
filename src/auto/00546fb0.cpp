#include "globals.h"
#include "hook.h"

// ---------------------------------------------------------------------------
// 0x00546DF0 - __cdecl helper (rets 0, i.e. the caller cleans the stack).
// The wrapper below pushes five dwords before the call (add esp, 0x14), so the
// helper effectively receives five stack arguments:
//   arg1 = a1    (pushed last)
//   arg2 = a2
//   arg3 = a3
//   arg4 = self  (incoming ecx value)
//   arg5 = a3    (pushed first)
// ---------------------------------------------------------------------------
CSF_FUNC(void*, __cdecl, CopyConstructRecordRange_546DF0, 0x546DF0,
         (void* a1, void* a2, void* a3, void* a4, void* a5));

// ---------------------------------------------------------------------------
// 0x00546FB0 - thin forwarder.  It is a __thiscall-style entry point
// (ecx = self, unused edx) with three DWORD stack arguments, hence the
// original's `ret 0xC` is reproduced exactly by __fastcall.
//
// It duplicates the third stack argument and splices the incoming ecx value
// between the first and second arguments:
//   pushes: a3, self, a3, a2, a1
//   call  : CopyConstructRecordRange_546DF0(a1, a2, a3, self, a3)
// ---------------------------------------------------------------------------
static void* __fastcall ForwardWithSelfInserted(void* self, void* /*edx*/,
                                                void* a1, void* a2, void* a3)
{
    return CopyConstructRecordRange_546DF0(a1, a2, a3, self, a3);
}

HOOK(0x546FB0, ForwardWithSelfInserted, "auto");
