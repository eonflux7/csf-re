#include "globals.h"
#include "hook.h"

// ---------------------------------------------------------------------------
// 0x00452760 - __cdecl callee (rets 0, the caller cleans the stack).
// The wrapper pushes five dwords and removes them with `add esp, 0x14`:
//   arg1 = [ebp+8]    (first stack argument of the forwarder)
//   arg2 = [ebp+0xC]  (second stack argument)
//   arg3 = [ebp+0x10] (third stack argument)
//   arg4 = self       (the incoming ecx value)
//   arg5 = [ebp+0x10] (third stack argument again)
// ---------------------------------------------------------------------------
CSF_FUNC(uint32_t*, __cdecl, RangeRecordHelper_452760, 0x452760,
         (uint32_t* a1, uint32_t* a2, uint32_t* a3, int a4, int a5));

// ---------------------------------------------------------------------------
// 0x00453930 - thin forwarder.  The original is __thiscall with ret 0xC; it
// forwards the incoming ecx as an extra stack argument, so it is written as
// __fastcall (ecx = self, unused edx) with three DWORD stack arguments, which
// gives the same callee-popped byte count of 0xC.
//   entry : ecx = self, stack args a1, a2, a3
//   pushes: a3, self, a3, a2, a1
//   call  : RangeRecordHelper_452760(a1, a2, a3, self, a3)
// ---------------------------------------------------------------------------
static uint32_t* __fastcall ForwardToRangeRecordHelper(void* self, void* /*edx*/,
                                                       uint32_t* a1, uint32_t* a2,
                                                       uint32_t* a3)
{
    return RangeRecordHelper_452760(a1, a2, a3,
                                    reinterpret_cast<int>(self),
                                    reinterpret_cast<int>(a3));
}

HOOK(0x453930, ForwardToRangeRecordHelper, "auto");
