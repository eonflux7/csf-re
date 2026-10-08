#include "globals.h"
#include "hook.h"

// ---------------------------------------------------------------------------
// 0x0042E3B0 - __cdecl callee (rets 0, the caller cleans the stack).
// The wrapper pushes five dwords and removes them with `add esp, 0x14`, so the
// callee effectively consumes five stack arguments:
//   arg1 = a1      (pushed last)
//   arg2 = a2
//   arg3 = a3
//   arg4 = self    (the incoming ecx value, pushed as a plain value)
//   arg5 = a3      (pushed first, re-read from [ebp+0x10])
// ---------------------------------------------------------------------------
CSF_FUNC(int, __cdecl, HudRecordRangeHelper_42E3B0, 0x42E3B0,
         (int a1, int a2, int a3, void* a4, int a5));

// ---------------------------------------------------------------------------
// 0x00431CF0 - thin forwarder.  The original is __stdcall with ret 0xC, but it
// also forwards the incoming ecx as an extra stack argument, so it is written
// as __fastcall (ecx = self, unused edx) with three DWORD stack arguments,
// which gives the same callee-popped byte count of 0xC.
//   entry : ecx = self, stack args a1, a2, a3
//   pushes: a3, self, a3, a2, a1
//   call  : HudRecordRangeHelper_42E3B0(a1, a2, a3, self, a3)
// ---------------------------------------------------------------------------
static int __fastcall Hud_ForwardToRecordRangeHelper(void* self, void* /*edx*/,
                                                     int a1, int a2, int a3)
{
    return HudRecordRangeHelper_42E3B0(a1, a2, a3, self, a3);
}

HOOK(0x431CF0, Hud_ForwardToRecordRangeHelper, "auto");
