#include "globals.h"
#include "hook.h"

// ---------------------------------------------------------------------------
// 0x00755CE0 - __cdecl callee (rets 0, the caller cleans the stack) that
// returns a DWORD pointer in eax. The wrapper pushes five dwords and removes
// them with `add esp, 0x14`, so the callee consumes five stack arguments:
//   arg1 = a1      (pushed last)
//   arg2 = a2
//   arg3 = a3
//   arg4 = self    (the incoming ecx value, pushed as a plain value)
//   arg5 = a3      (pushed first, re-read from [ebp+0x10])
// ---------------------------------------------------------------------------
CSF_FUNC(uint32_t*, __cdecl, GameConfig_SetupHelper_755CE0, 0x00755CE0,
         (uint32_t* a1, uint32_t* a2, uint32_t* a3, int a4, int a5));

// ---------------------------------------------------------------------------
// 0x00755F00 - thin forwarder. The original is __thiscall with ret 0xC, so it
// is written as __fastcall (ecx = self, unused edx) with three DWORD stack
// arguments, which gives the same callee-popped byte count of 0xC.
//   entry : ecx = self, stack args a1, a2, a3
//   pushes: a3, self, a3, a2, a1
//   call  : GameConfig_SetupHelper_755CE0(a1, a2, a3, self, a3)
// ---------------------------------------------------------------------------
static uint32_t* __fastcall GameConfig_ForwardToSetupHelper(void* self, void* /*edx*/,
                                                            uint32_t* a1, uint32_t* a2,
                                                            uint32_t* a3)
{
    return GameConfig_SetupHelper_755CE0(a1, a2, a3,
                                         reinterpret_cast<int>(self),
                                         reinterpret_cast<int>(a3));
}

HOOK(0x755F00, GameConfig_ForwardToSetupHelper, "auto");
