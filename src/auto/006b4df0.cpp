#include <cstdint>

#include "globals.h"
#include "hook.h"

// ---------------------------------------------------------------------------
// 0x006B42C0 - __cdecl callee (rets 0, the caller cleans the stack).
// The wrapper pushes five dwords and removes them with `add esp, 0x14`:
//   arg1 = a1      (source, pushed last)
//   arg2 = a2      (sourceEnd)
//   arg3 = a3      (destination)
//   arg4 = self    (the incoming ecx value)
//   arg5 = a3      (pushed first, re-read from [ebp+0x10])
// ---------------------------------------------------------------------------
CSF_FUNC(int, __cdecl, CopyDwordRangeToDestination_6B42C0, 0x006B42C0,
         (std::uint32_t source, std::uint32_t sourceEnd, std::uint32_t destination,
          std::uint32_t, std::uint32_t));

// ---------------------------------------------------------------------------
// 0x006B4DF0 - thin forwarder.  The original is __thiscall with ret 0xC; the
// incoming ecx is forwarded as an extra stack argument, so it is written as
// __fastcall (ecx = self, unused edx) with three DWORD stack arguments, which
// gives the same callee-popped byte count of 0xC.
//   entry : ecx = self, stack args a1, a2, a3
//   pushes: a3, self, a3, a2, a1
//   call  : CopyDwordRangeToDestination_6B42C0(a1, a2, a3, self, a3)
// ---------------------------------------------------------------------------
static int __fastcall ForwardDwordRangeCopyWithSelf(void* self, void* /*edx*/,
                                                    int a1, int a2, int a3)
{
    const std::uint32_t selfValue =
        static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(self));
    return CopyDwordRangeToDestination_6B42C0(static_cast<std::uint32_t>(a1),
                                              static_cast<std::uint32_t>(a2),
                                              static_cast<std::uint32_t>(a3),
                                              selfValue,
                                              static_cast<std::uint32_t>(a3));
}

HOOK(0x6B4DF0, ForwardDwordRangeCopyWithSelf, "auto");
