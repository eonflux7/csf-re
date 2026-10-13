#include "globals.h"
#include "hook.h"

// ---------------------------------------------------------------------------
// 0x006B3EA0 - __cdecl callee (rets 0, the caller cleans the stack).
// The wrapper pushes five dwords and removes them with `add esp, 0x14`:
//   arg1 = a1      (sourceBegin, pushed last)
//   arg2 = a2      (sourceEnd)
//   arg3 = a3      (destination)
//   arg4 = self    (the incoming ecx value)
//   arg5 = a3      (pushed first, re-read from [ebp+0x10])
// ---------------------------------------------------------------------------
CSF_FUNC(int, __cdecl, InitEntryRangeFromSources, 0x006B3EA0,
         (void** sourceBegin, void** sourceEnd, int destination, void* contextThis, int));

// ---------------------------------------------------------------------------
// 0x006B4E10 - thin forwarder.  The original is __thiscall with ret 0xC; the
// incoming ecx is forwarded as an extra stack argument, so it is written as
// __fastcall (ecx = self, unused edx) with three stack arguments, which gives
// the same callee-popped byte count of 0xC.
//   entry : ecx = self, stack args a1, a2, a3
//   pushes: a3, self, a3, a2, a1
//   call  : InitEntryRangeFromSources(a1, a2, a3, self, a3)
// ---------------------------------------------------------------------------
static int __fastcall ForwardInitEntryRangeWithSelf(void* self, void* /*edx*/,
                                                    void** a1, void** a2, int a3)
{
    return InitEntryRangeFromSources(a1, a2, a3, self, a3);
}

HOOK(0x6B4E10, ForwardInitEntryRangeWithSelf, "auto");
