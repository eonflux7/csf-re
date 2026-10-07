#include <cstdint>
#include "globals.h"
#include "hook.h"

// Callee at 0x65E950: __stdcall with 3 int args (rets 12 per disassembly).
CSF_FUNC(int, __stdcall, SharedDispatch_65E950, 0x65E950, (int a1, int a2, int a3));

// Thunk: forwards its three arguments unchanged to the shared dispatcher.
// Original body:  push ebp / mov ebp,esp / pop ebp / jmp 0x65E950
// The callee pops the 12 bytes of stack args, so the thunk also reports ret 12 (__stdcall, 3 ints).
static int __stdcall ForwardToSharedDispatch(int a1, int a2, int a3)
{
    return SharedDispatch_65E950(a1, a2, a3);
}

HOOK(0x0074E690, ForwardToSharedDispatch, "auto");
