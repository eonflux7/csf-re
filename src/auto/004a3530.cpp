#include "globals.h"
#include "hook.h"

// ---------------------------------------------------------------------------
// 0x004A3230 - __cdecl callee (rets 0, the caller cleans the stack).
// The wrapper pushes five dwords and removes them with `add esp, 0x14`:
//   arg1 = a1      (pushed last)
//   arg2 = a2
//   arg3 = a3
//   arg4 = self    (the incoming ecx value)
//   arg5 = a3      (pushed first, re-read from [ebp+0x10])
// ---------------------------------------------------------------------------
CSF_FUNC(uint32_t*, __cdecl, sub_4A3230, 0x004A3230,
         (uint32_t* a1, uint32_t* a2, uint32_t* a3, int a4, int a5));

// ---------------------------------------------------------------------------
// 0x004A3530 - thin forwarder.  The original is __thiscall with ret 0xC; the
// incoming ecx is forwarded as an extra stack argument, so it is written as
// __fastcall (ecx = self, unused edx) with three pointer stack arguments,
// which gives the same callee-popped byte count of 0xC.
//   entry : ecx = self, stack args a1 ([ebp+8]), a2 ([ebp+0xC]), a3 ([ebp+0x10])
//   pushes: a3, self, a3, a2, a1
//   call  : sub_4A3230(a1, a2, a3, self, a3)
//   return: the callee's eax, unchanged
// ---------------------------------------------------------------------------
static uint32_t* __fastcall PointerTriple_ForwardTo4A3230(void* self, void* /*edx*/,
                                                          uint32_t* a1, uint32_t* a2,
                                                          uint32_t* a3)
{
    return sub_4A3230(a1, a2, a3, (int)self, (int)a3);
}

HOOK(0x4A3530, PointerTriple_ForwardTo4A3230, "auto");
