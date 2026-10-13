#include <cstdint>
#include "globals.h"
#include "hook.h"

// Callee at 0x00782A70 is __cdecl with four forwarded values: the first stack
// argument, the second stack argument, the incoming ECX (context), and the
// second argument repeated.
CSF_FUNC(int, __cdecl, Forward_ProcessPair782A70, 0x00782A70,
         (int* a1, int* a2, int a3, int a4));

// 0x00782C50: __thiscall thunk, callee-cleaned (ret 8). The original pushes
// (a2, ECX, a2, a1) so the callee sees (a1, a2, ECX, a2). ECX arrives as the
// first __fastcall parameter and the two stack arguments are callee-cleaned.
static int __fastcall ForwardPairWithContext782C50(
    void* context, void* /*edx*/, int* firstArgument, int* secondArgument)
{
    return Forward_ProcessPair782A70(
        firstArgument,
        secondArgument,
        static_cast<int>(reinterpret_cast<std::uintptr_t>(context)),
        static_cast<int>(reinterpret_cast<std::uintptr_t>(secondArgument)));
}

HOOK(0x00782C50, ForwardPairWithContext782C50, "auto");
