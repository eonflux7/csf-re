#include "globals.h"
#include "hook.h"

// ---------------------------------------------------------------------------
// 0x0046EF80 - __cdecl callee (rets 0, the caller cleans the stack).
// The wrapper pushes five dwords and removes them with `add esp, 0x14`:
//   arg1 = a1      (pushed last)
//   arg2 = a2
//   arg3 = a3
//   arg4 = self    (the incoming ecx value, pushed as a plain value)
//   arg5 = a3      (pushed first, re-read from [ebp+0x10])
// ---------------------------------------------------------------------------
CSF_FUNC(uint32_t*, __cdecl, MissionScript_ParseEntryHelper, 0x0046EF80,
         (int a1, int a2, uint32_t* a3, int a4, int a5));

// ---------------------------------------------------------------------------
// 0x0046F430 - thin forwarder.  The original is __thiscall with ret 0xC, but
// its only effect is to forward the incoming ecx as an extra stack argument.
// It is written as __fastcall (ecx = self, unused edx) with three DWORD stack
// arguments, which gives the same callee-popped byte count of 0xC.
//   entry : ecx = self, stack args a1, a2, a3
//   pushes: a3, self, a3, a2, a1
//   call  : MissionScript_ParseEntryHelper(a1, a2, a3, self, a3)
//   return: the callee's result, passed through unchanged in eax
// ---------------------------------------------------------------------------
static uint32_t* __fastcall MissionScript_ForwardToParseEntryHelper(void* self, void* /*edx*/,
                                                                    int a1, int a2, uint32_t* a3)
{
    return MissionScript_ParseEntryHelper(a1, a2, a3,
                                          static_cast<int>(reinterpret_cast<uintptr_t>(self)),
                                          static_cast<int>(reinterpret_cast<uintptr_t>(a3)));
}

HOOK(0x46F430, MissionScript_ForwardToParseEntryHelper, "auto");
