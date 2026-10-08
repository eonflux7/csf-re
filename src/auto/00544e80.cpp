#include "globals.h"
#include "hook.h"

// ---------------------------------------------------------------------------
// 0x00544CA0 - __cdecl helper (rets 0, i.e. the caller cleans the stack).
// The forwarder below pushes five dwords before the call (add esp, 0x14), so
// the helper receives five stack arguments:
//   arg1 = a1    (pushed last)
//   arg2 = a2
//   arg3 = a3
//   arg4 = this  (incoming ecx value)
//   arg5 = a3    (pushed first)
// ---------------------------------------------------------------------------
CSF_FUNC(int, __cdecl, CopyConstructRecordRange_544CA0, 0x544CA0,
         (int a1, int a2, int a3, int a4, int a5));

// ---------------------------------------------------------------------------
// 0x00544E80 - thin forwarder.  ecx carries the object value, which is passed
// on as an extra argument; hence the __thiscall-style __fastcall (ecx = self,
// unused edx).  The three DWORD stack arguments mean the callee-popped byte
// count of the original (ret 0xC) is reproduced exactly.
//   entry : ecx = self, stack args a1, a2, a3
//   pushes: a3, self, a3, a2, a1
//   call  : CopyConstructRecordRange_544CA0(a1, a2, a3, self, a3)
// ---------------------------------------------------------------------------
static int __fastcall CMenuSaveGame_ForwardToHelper(void* self, void* /*edx*/,
                                                    int a1, int a2, int a3)
{
    return CopyConstructRecordRange_544CA0(a1, a2, a3, reinterpret_cast<int>(self), a3);
}

HOOK(0x544E80, CMenuSaveGame_ForwardToHelper, "auto");
