#include <cstdint>
#include "globals.h"
#include "hook.h"

// Callee at 0x00775200 is __cdecl and takes four values: the first stack
// argument, the second stack argument, the incoming ECX (this), and the
// second stack argument repeated.
CSF_FUNC(int, __cdecl, LevelIndexTable_Destroy, 0x00775200,
         (int* a1, int* a2, int a3, int a4));

// 0x00775960: __thiscall thunk, callee-cleaned (ret 8). The original pushes
// (a2, ECX, a2, a1), so the callee sees (a1, a2, this, a2). ECX arrives as the
// first __fastcall parameter and the two stack arguments are callee-cleaned.
static int __fastcall LevelIndexTable_DestroyThunk(
    void* context, void* /*edx*/, int* firstArgument, int* secondArgument)
{
    return LevelIndexTable_Destroy(
        firstArgument,
        secondArgument,
        static_cast<int>(reinterpret_cast<std::uintptr_t>(context)),
        reinterpret_cast<int>(secondArgument));
}

HOOK(0x00775960, LevelIndexTable_DestroyThunk, "auto");
