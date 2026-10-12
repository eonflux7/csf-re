#include <cstdint>
#include "globals.h"
#include "hook.h"

// Callee at 0x007752E0 is __cdecl with four forwarded values: the first stack
// argument, the second stack argument, the incoming ECX (context), and the
// second argument repeated.
CSF_FUNC(int, __cdecl, LevelBlock44_Destroy, 0x007752E0,
         (int a1, int a2, int a3, int a4));

// 0x007759B0: __thiscall thunk, callee-cleaned (ret 8). The original pushes
// (a2, ECX, a2, a1) so the callee sees (a1, a2, ECX, a2). ECX arrives as the
// first __fastcall parameter and the two stack arguments are callee-cleaned.
static int __fastcall LevelBlock44_ForwardDestroy(
    void* context, void* /*edx*/, int firstArgument, int secondArgument)
{
    return LevelBlock44_Destroy(
        firstArgument,
        secondArgument,
        static_cast<int>(reinterpret_cast<std::uintptr_t>(context)),
        secondArgument);
}

HOOK(0x007759B0, LevelBlock44_ForwardDestroy, "auto");
