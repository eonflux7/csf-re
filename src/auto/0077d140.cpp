#include <cstdint>
#include "globals.h"
#include "hook.h"

// ---------------------------------------------------------------------------
// 0x00778A00 - __cdecl callee (rets 0, the caller cleans the stack).
// The wrapper pushes five dwords and removes them with `add esp, 0x14`:
//   arg1 = a1, arg2 = a2, arg3 = a3, arg4 = self (incoming ecx), arg5 = a3
// ---------------------------------------------------------------------------
CSF_FUNC(int, __cdecl, GameDataDefs_RecordRangeHelper_778A00, 0x778A00,
         (int a1, int a2, int a3, int a4, int a5));

// ---------------------------------------------------------------------------
// 0x0077D140 - thin forwarder.  The original is __thiscall with ret 0xC.
// It forwards the incoming ecx as an extra stack argument, so it is written
// as __fastcall (ecx = self, unused edx) with three DWORD stack arguments,
// giving the same callee-popped byte count of 0xC.
//   entry : ecx = self, stack args a1, a2, a3
//   pushes: a3, self, a3, a2, a1
//   call  : GameDataDefs_RecordRangeHelper_778A00(a1, a2, a3, self, a3)
// ---------------------------------------------------------------------------
static int __fastcall GameDataDefs_ForwardToRecordRangeHelper(void* self, void* /*edx*/,
                                                              int a1, int a2, int a3)
{
    return GameDataDefs_RecordRangeHelper_778A00(a1, a2, a3,
                                                 static_cast<int>(reinterpret_cast<uintptr_t>(self)),
                                                 a3);
}

HOOK(0x77D140, GameDataDefs_ForwardToRecordRangeHelper, "auto");
