#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"

// ---------------------------------------------------------------------------
// 0x0069B970 - __cdecl callee (rets 0, the caller cleans the stack).
// The wrapper pushes five values and removes them with `add esp, 0x14`:
//   a1 = first pointer argument   (pushed last)
//   a2 = second pointer argument
//   a3 = third argument
//   a4 = self (the incoming ecx value)
//   a5 = a3 again                 (pushed first)
// ---------------------------------------------------------------------------
CSF_FUNC(int, __cdecl, sub_69B970, 0x0069B970,
         (uint32_t* a1, uint32_t* a2, int a3, int a4, int a5));

// ---------------------------------------------------------------------------
// 0x0069BD70 - thin forwarder. The original is __thiscall with ret 0xC; the
// incoming ecx is forwarded as an extra stack argument, so it is written as
// __fastcall (ecx = self, unused edx) with three DWORD stack arguments, which
// gives the same callee-popped byte count of 0xC.
//   entry : ecx = self, stack args a1, a2, a3
//   pushes: a3, self, a3, a2, a1
//   call  : sub_69B970(a1, a2, a3, self, a3)
// ---------------------------------------------------------------------------
static int __fastcall ForwardWithSelfTo69B970(void* self, void* /*edx*/,
                                              uint32_t* a1, uint32_t* a2, int a3)
{
    const int selfAsInt = static_cast<int>(reinterpret_cast<uintptr_t>(self));
    return sub_69B970(a1, a2, a3, selfAsInt, a3);
}

HOOK(0x69BD70, ForwardWithSelfTo69B970, "auto");
