#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// ---------------------------------------------------------------------------
// 0x00776970 - __cdecl callee (rets 0, the caller cleans the stack).
// The wrapper pushes five dwords and removes them with `add esp, 0x14`, so the
// callee consumes five stack arguments:
//   arg1 = a1      (pushed last)
//   arg2 = a2
//   arg3 = a3
//   arg4 = self    (the incoming ecx value, pushed as a plain value)
//   arg5 = a3      (pushed first, re-read from [ebp+0x10])
// The two string-object arguments are passed as opaque pointers, since they
// are only forwarded and never dereferenced here.
// ---------------------------------------------------------------------------
CSF_FUNC(int, __cdecl, GameDataDefs_ParseStringEntry, 0x00776970,
         (void* a1, void* a2, int a3, int a4, int a5));

// ---------------------------------------------------------------------------
// 0x00778100 - thin forwarder.  The original is __thiscall with ret 0xC.  It
// takes the incoming ecx as `this` and forwards it as an extra stack argument,
// so it is written as __fastcall (ecx = self, unused edx) with three DWORD
// stack arguments, which gives the same callee-popped byte count of 0xC.
//   entry : ecx = self, stack args a1, a2, a3
//   pushes: a3, self, a3, a2, a1
//   call  : GameDataDefs_ParseStringEntry(a1, a2, a3, self, a3)
// ---------------------------------------------------------------------------
static int __fastcall GameDataDefs_ForwardToParseStringEntry(S_0051aa70* self, void* /*edx*/,
                                                             void* a1,
                                                             void* a2,
                                                             int a3)
{
    return GameDataDefs_ParseStringEntry(a1, a2, a3, reinterpret_cast<int>(self), a3);
}

HOOK(0x778100, GameDataDefs_ForwardToParseStringEntry, "auto");
