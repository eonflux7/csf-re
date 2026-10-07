#include <cstdint>
#include "globals.h"
#include "hook.h"

// Library routine at 0x7A3090, reached by tail-jump from the target wrapper.
// Four 32-bit arguments, __stdcall (callee pops 16 bytes), returns int.
CSF_FUNC(int, __stdcall, Lib_7A3090, 0x7A3090, (int a1, int a2, int a3, int a4));

// Pure forwarding thunk: `push ebp; mov ebp, esp; pop ebp; jmp 0x7A3090`.
static int __stdcall ForwardToLib7A3090(int a1, int a2, int a3, int a4)
{
    return Lib_7A3090(a1, a2, a3, a4);
}

HOOK(0x750C60, ForwardToLib7A3090, "auto");
