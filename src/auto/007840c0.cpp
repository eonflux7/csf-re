#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Callee at 0x00783670 is __cdecl with four forwarded values: the two original
// stack arguments, the incoming ECX (context), and the second argument repeated.
CSF_FUNC(int, __cdecl, WeaponDefs_ApplyPairWithContext, 0x00783670,
         (int a1, int a2, int a3, int a4));

// 0x007840C0: __thiscall-style forwarder (ecx = context, two stack args, ret 8).
// The original pushes (arg1, arg0-as-second, ECX, arg1) so the callee sees
// (arg0, arg1, ECX, arg1); the context pointer is passed as a plain int.
static int __fastcall WeaponDefs_ForwardPairWithContext(
    void* context, void* /*edx*/, int firstArgument, int secondArgument)
{
    return WeaponDefs_ApplyPairWithContext(
        firstArgument,
        secondArgument,
        static_cast<int>(reinterpret_cast<std::uintptr_t>(context)),
        secondArgument);
}

HOOK(0x007840C0, WeaponDefs_ForwardPairWithContext, "auto");
