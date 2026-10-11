#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// ---------------------------------------------------------------------------
// 0x00410C20 - __cdecl callee (rets 0, the caller cleans the stack).
// The wrapper pushes five dwords and removes them with `add esp, 0x14`:
//   arg1 = a1      (pushed last)
//   arg2 = a2
//   arg3 = a3
//   arg4 = self    (the incoming ecx value, pushed as a plain value)
//   arg5 = a3      (pushed first, re-read from [ebp+0x10])
// ---------------------------------------------------------------------------
CSF_FUNC(int, __cdecl, StringRangeHelper_410C20, 0x410C20,
         (void* a1, void* a2, int a3, int a4, int a5));

// ---------------------------------------------------------------------------
// 0x00410E50 - thin forwarder.  The original is __thiscall with ret 0xC, but
// it also forwards the incoming ecx as an extra stack argument, so it is
// written as __fastcall (ecx = self, unused edx) with three stack arguments,
// which gives the same callee-popped byte count of 0xC.
//   entry : ecx = self, stack args a1, a2, a3
//   pushes: a3, self, a3, a2, a1
//   call  : StringRangeHelper_410C20(a1, a2, a3, self, a3)
// ---------------------------------------------------------------------------
static int __fastcall StringBase_ForwardToRangeHelper(S_0046bea0* self, void* /*edx*/,
                                                      void* a1, void* a2, int a3)
{
    return StringRangeHelper_410C20(a1, a2, a3,
                                    static_cast<int>(reinterpret_cast<std::uintptr_t>(self)),
                                    a3);
}

HOOK(0x410E50, StringBase_ForwardToRangeHelper, "auto");
