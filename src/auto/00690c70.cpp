#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Bound by address; the callee takes (self in ecx, edx, then stack: a2, a3, uint64 a4).
CSF_FUNC(double, __fastcall, sub_6903E0, 0x006903E0, (void* self, void* edx, int a2, int a3, uint64_t a4));

// Forwards a 64-bit value to 0x6903E0. The original takes its `this` from ecx (the
// caller's a3, not the object), passes a2 unchanged, passes the address of this+8 as the
// third argument, and passes the 64-bit value pointed to by a4 as the fourth.
// The original tail-returns the callee's st0 result, so this wrapper returns the same double.
static double __fastcall C_00861a00_ForwardAllocation(C_00861a00* self, void* /*edx*/, int a2, int a3, const uint64_t* a4)
{
    void* calleeSelf = reinterpret_cast<void*>(static_cast<uintptr_t>(static_cast<uint32_t>(a3)));
    int blockAddress = static_cast<int>(reinterpret_cast<uintptr_t>(reinterpret_cast<uint8_t*>(self) + 8));
    return sub_6903E0(calleeSelf, nullptr, a2, blockAddress, *a4);
}

HOOK(0x690C70, C_00861a00_ForwardAllocation, "auto");
