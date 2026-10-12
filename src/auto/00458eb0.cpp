#include "globals.h"
#include "hook.h"

// ---------------------------------------------------------------------------
// 0x00458530 - __cdecl callee (rets 0, the caller cleans the stack).
// The wrapper pushes five dwords and removes them with `add esp, 0x14`:
//   arg1 = a1      (pushed last)
//   arg2 = a2
//   arg3 = a3
//   arg4 = self    (the incoming ecx value)
//   arg5 = a3      (pushed first, re-read from [ebp+0x10])
// Returns a char* in eax, which the wrapper passes through unchanged.
// ---------------------------------------------------------------------------
CSF_FUNC(char*, __cdecl, HudStatusIcons_ResolveIconSlot, 0x458530,
         (char* a1, char* a2, char* a3, void* a4, char* a5));

// ---------------------------------------------------------------------------
// 0x00458EB0 - thin forwarder.  The original is __thiscall with ret 0xC; the
// incoming ecx is forwarded as an extra stack argument, so it is written as
// __fastcall (ecx = self, unused edx) with three stack arguments, which gives
// the same callee-popped byte count of 0xC.
//   entry : ecx = self, stack args a1, a2, a3
//   pushes: a3, self, a3, a2, a1
//   call  : HudStatusIcons_ResolveIconSlot(a1, a2, a3, self, a3)
// ---------------------------------------------------------------------------
static char* __fastcall ForwardWithSelfToResolveIconSlot(void* self, void* /*edx*/,
                                                         char* a1, char* a2, char* a3)
{
    return HudStatusIcons_ResolveIconSlot(a1, a2, a3, self, a3);
}

HOOK(0x458EB0, ForwardWithSelfToResolveIconSlot, "auto");
