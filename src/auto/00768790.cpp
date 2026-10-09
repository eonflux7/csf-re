#include "globals.h"
#include "hook.h"

// ---------------------------------------------------------------------------
// 0x00768600 - __cdecl callee (rets 0, the caller cleans the stack).
// The wrapper pushes five dwords and removes them with `add esp, 0x14`, so the
// callee effectively consumes five stack arguments:
//   arg1 = a1      (pushed last)
//   arg2 = a2
//   arg3 = a3
//   arg4 = self    (the incoming ecx value, pushed as a plain value)
//   arg5 = a3      (pushed first, re-read from [ebp+0x10])
// ---------------------------------------------------------------------------
CSF_FUNC(uint32_t*, __cdecl, InputBindings_BuildActionEntry_768600, 0x00768600,
         (uint32_t* a1, uint32_t* a2, uint32_t* a3, int a4, int a5));

// ---------------------------------------------------------------------------
// 0x00768790 - thin forwarder.  The original is __thiscall with ret 0xC, and it
// forwards the incoming ecx as an extra stack argument, so it is written as
// __fastcall (ecx = self, unused edx) with three DWORD stack arguments, which
// gives the same callee-popped byte count of 0xC.
//   entry : ecx = self, stack args a1, a2, a3
//   pushes: a3, self, a3, a2, a1
//   call  : InputBindings_BuildActionEntry_768600(a1, a2, a3, self, a3)
// ---------------------------------------------------------------------------
static uint32_t* __fastcall InputBindings_ForwardToBuildActionEntry(void* self, void* /*edx*/,
                                                                    uint32_t* a1, uint32_t* a2,
                                                                    uint32_t* a3)
{
    return InputBindings_BuildActionEntry_768600(a1, a2, a3,
                                                 (int)(intptr_t)self,
                                                 (int)(intptr_t)a3);
}

HOOK(0x768790, InputBindings_ForwardToBuildActionEntry, "auto");
