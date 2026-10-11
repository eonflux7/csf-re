#include "globals.h"
#include "hook.h"

// ---------------------------------------------------------------------------
// 0x00762CD0 - __cdecl callee (rets 0, the caller cleans the stack).
// The wrapper pushes five dwords and removes them with `add esp, 0x14`:
//   arg1 = a1      (pushed last)
//   arg2 = a2
//   arg3 = a3
//   arg4 = self    (the incoming ecx value, pushed as a plain value)
//   arg5 = a3      (pushed first, re-read from [ebp+0x10])
// ---------------------------------------------------------------------------
CSF_FUNC(uint32_t*, __cdecl, CharacterDefs_RecordHelper_762CD0, 0x762CD0,
         (int a1, int a2, uint32_t* a3, int a4, int a5));

// ---------------------------------------------------------------------------
// 0x00762EB0 - thin forwarder.  The original is __thiscall-shaped with
// ret 0xC: ecx = self, three DWORD stack arguments.  It forwards the incoming
// ecx as an extra stack argument, so it is written as __fastcall (ecx = self,
// unused edx) with three stack arguments, giving the same callee-popped
// byte count of 0xC.
//   entry : ecx = self, stack args a1, a2, a3
//   pushes: a3, self, a3, a2, a1
//   call  : CharacterDefs_RecordHelper_762CD0(a1, a2, a3, self, a3)
// ---------------------------------------------------------------------------
static uint32_t* __fastcall CharacterDefs_ForwardToRecordHelper(void* self, void* /*edx*/,
                                                                int a1, int a2, uint32_t* a3)
{
    return CharacterDefs_RecordHelper_762CD0(a1, a2, a3,
                                             static_cast<int>(reinterpret_cast<uintptr_t>(self)),
                                             static_cast<int>(reinterpret_cast<uintptr_t>(a3)));
}

HOOK(0x762EB0, CharacterDefs_ForwardToRecordHelper, "auto");
