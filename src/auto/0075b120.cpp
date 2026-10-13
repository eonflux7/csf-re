#include "globals.h"
#include "hook.h"

// ---------------------------------------------------------------------------
// 0x0075AF40 - __cdecl callee (rets 0, the caller cleans the stack).
// The wrapper pushes five dwords and removes them with `add esp, 0x14`:
//   arg1 = a1      (pushed last)
//   arg2 = a2
//   arg3 = a3
//   arg4 = self    (the incoming ecx value)
//   arg5 = a3      (pushed first, re-read from [ebp+0x10])
// ---------------------------------------------------------------------------
CSF_FUNC(int, __cdecl, SoundDefs_ParseEntry, 0x0075AF40,
         (uint32_t* a1, uint32_t* a2, int a3, int a4, int a5));

// ---------------------------------------------------------------------------
// 0x0075B120 - thin forwarder.  The original is __thiscall with ret 0xC; the
// incoming ecx is forwarded as an extra stack argument, so it is written as
// __fastcall (ecx = self, unused edx) with three DWORD stack arguments, which
// gives the same callee-popped byte count of 0xC.
//   entry : ecx = self, stack args a1, a2, a3
//   pushes: a3, self, a3, a2, a1
//   call  : SoundDefs_ParseEntry(a1, a2, a3, self, a3)
// ---------------------------------------------------------------------------
static int __fastcall SoundDefs_ForwardWithSelfToParseEntry(void* self, void* /*edx*/,
                                                            uint32_t* a1, uint32_t* a2, int a3)
{
    return SoundDefs_ParseEntry(a1, a2, a3, reinterpret_cast<int>(self), a3);
}

HOOK(0x75B120, SoundDefs_ForwardWithSelfToParseEntry, "auto");
