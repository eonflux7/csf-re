#include "globals.h"
#include "hook.h"

// ---------------------------------------------------------------------------
// 0x006D9F30 - __cdecl callee (rets 0, the caller cleans the stack).
// The wrapper pushes five values and removes them with `add esp, 0x14`:
//   arg1 = a1      (pushed last)
//   arg2 = a2
//   arg3 = a3
//   arg4 = self    (the incoming ecx value)
//   arg5 = a3      (pushed first, re-read from [ebp+0x10])
// ---------------------------------------------------------------------------
CSF_FUNC(uint32_t*, __cdecl, sub_6D9F30, 0x006D9F30,
         (uint32_t* a1, uint32_t* a2, uint32_t* a3, int a4, int a5));

// ---------------------------------------------------------------------------
// 0x006DA670 - thin forwarder.  The original is __thiscall with ret 0xC; the
// incoming ecx is forwarded as an extra stack argument, so it is written as
// __fastcall (ecx = self, unused edx) with three DWORD stack arguments, which
// gives the same callee-popped byte count of 0xC.
//   entry : ecx = self, stack args a1, a2, a3
//   pushes: a3, self, a3, a2, a1
//   call  : sub_6D9F30(a1, a2, a3, self, a3)
// ---------------------------------------------------------------------------
static uint32_t* __fastcall ForwardWithSelfToSub6D9F30(void* self, void* /*edx*/,
                                                       uint32_t* a1, uint32_t* a2,
                                                       uint32_t* a3)
{
    return sub_6D9F30(a1, a2, a3, reinterpret_cast<int>(self), reinterpret_cast<int>(a3));
}

HOOK(0x6DA670, ForwardWithSelfToSub6D9F30, "auto");
