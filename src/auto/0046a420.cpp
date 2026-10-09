#include "globals.h"
#include "hook.h"

// ---------------------------------------------------------------------------
// 0x0046A100 - __cdecl callee (rets 0, the caller cleans the stack).
// The wrapper pushes five dwords and removes them with `add esp, 0x14`:
//   arg1 = a1      (pushed last)
//   arg2 = a2
//   arg3 = a3      (the char* argument, pushed as a plain dword)
//   arg4 = self    (the incoming ecx value, pushed as a plain value)
//   arg5 = a3      (pushed first, re-read from [ebp+0x10])
// ---------------------------------------------------------------------------
CSF_FUNC(char*, __cdecl, sub_46A100, 0x0046A100,
         (int a1, int a2, char* a3, int a4, int a5));

// ---------------------------------------------------------------------------
// 0x0046A420 - thin forwarder.  The original is __thiscall-like with ret 0xC:
// ecx carries the object pointer, which is forwarded as an extra stack
// argument.  It is written as __fastcall (ecx = self, unused edx) with three
// DWORD stack arguments, giving the same callee-popped byte count of 0xC.
//   entry : ecx = self, stack args a1, a2, a3
//   pushes: a3, self, a3, a2, a1
//   call  : sub_46A100(a1, a2, a3, self, a3)
//   return: the callee's char* result, passed through unchanged
// ---------------------------------------------------------------------------
static char* __fastcall MissionScript_ForwardToParseHelper(void* self, void* /*edx*/,
                                                           int a1, int a2, char* a3)
{
    return sub_46A100(a1, a2, a3,
                      static_cast<int>(reinterpret_cast<uintptr_t>(self)),
                      static_cast<int>(reinterpret_cast<uintptr_t>(a3)));
}

HOOK(0x46A420, MissionScript_ForwardToParseHelper, "auto");
