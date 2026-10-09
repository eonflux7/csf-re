#include "globals.h"
#include "hook.h"

// ---------------------------------------------------------------------------
// 0x00768F30 - __cdecl callee (rets 0, the caller cleans the stack).
// The wrapper pushes five dwords and removes them with `add esp, 0x14`, so the
// callee consumes five stack arguments:
//   arg1 = a1      (pushed last)
//   arg2 = a2
//   arg3 = a3
//   arg4 = self    (the incoming ecx value, pushed as a plain value)
//   arg5 = a3      (pushed first, re-read from [ebp+0x10])
// ---------------------------------------------------------------------------
CSF_FUNC(uint32_t*, __cdecl, InputBindings_ActionHelper_768F30, 0x768F30,
         (uint32_t* a1, uint32_t* a2, uint32_t* a3, int a4, int a5));

// ---------------------------------------------------------------------------
// 0x007697F0 - thin forwarder.  The original is __thiscall with ret 0xC; the
// incoming ecx is forwarded as an extra stack argument, so it is written as
// __fastcall (ecx = self, unused edx) with three DWORD stack arguments, which
// gives the same callee-popped byte count of 0xC.  The callee's return value
// (eax) is passed straight through.
//   entry : ecx = self, stack args a1, a2, a3
//   pushes: a3, self, a3, a2, a1
//   call  : InputBindings_ActionHelper_768F30(a1, a2, a3, self, a3)
// ---------------------------------------------------------------------------
static uint32_t* __fastcall InputBindings_ForwardToActionHelper(void* self, void* /*edx*/,
                                                                uint32_t* a1, uint32_t* a2,
                                                                uint32_t* a3)
{
    return InputBindings_ActionHelper_768F30(a1, a2, a3, (int)self, (int)a3);
}

HOOK(0x7697F0, InputBindings_ForwardToActionHelper, "auto");
