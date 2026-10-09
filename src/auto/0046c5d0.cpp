#include <cstdint>
#include "globals.h"
#include "hook.h"

// Callee at 0x0046C190 is __cdecl with four forwarded values: the two original
// stack arguments, the incoming ECX (context), and the second argument repeated.
CSF_FUNC(int, __cdecl, MissionScript_ProcessValuePair, 0x0046C190,
         (int a1, int a2, int a3, int a4));

// 0x0046C5D0: __thiscall with two stack arguments, callee-cleaned (ret 8).
// Original pushes (arg1, arg0-as-second, ECX, arg1) so the callee sees
// (arg0, arg1, ECX, arg1). ECX arrives as the first __fastcall parameter.
static int __fastcall MissionScript_ForwardValuePairWithContext(
    void* context, void* /*edx*/, int a1, int a2)
{
    return MissionScript_ProcessValuePair(
        a1,
        a2,
        static_cast<int>(reinterpret_cast<std::uintptr_t>(context)),
        a2);
}

HOOK(0x0046C5D0, MissionScript_ForwardValuePairWithContext, "auto");
