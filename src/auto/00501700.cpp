#include <cstdint>
#include "globals.h"
#include "hook.h"

// Callee at 0x00500D60 is __cdecl with four values: the first stack argument,
// the second stack argument, the incoming ECX (context), and the second stack
// argument repeated.
CSF_FUNC(int, __cdecl, SceneLoader_DispatchArgumentsWithContext, 0x00500D60,
         (int a1, int a2, int a3, int a4));

// 0x00501700: __thiscall method with two stack arguments (ret 8). ECX arrives as
// the first __fastcall parameter; the original pushes (arg0, arg1, ECX, arg1) in
// that order so the callee sees (arg0, arg1, ECX, arg1).
static int __fastcall SceneLoader_ForwardArgumentsWithContext(
    void* context, void* /*edx*/, int firstArgument, int secondArgument)
{
    return SceneLoader_DispatchArgumentsWithContext(
        firstArgument,
        secondArgument,
        static_cast<int>(reinterpret_cast<std::uintptr_t>(context)),
        secondArgument);
}

HOOK(0x00501700, SceneLoader_ForwardArgumentsWithContext, "auto");
