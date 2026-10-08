#include <cstdint>
#include "globals.h"
#include "hook.h"

// Library routine at 0x7A3090, reached by tail-jump from the target wrapper.
// Four 32-bit arguments, __stdcall (callee pops 16 bytes), returns int.
CSF_FUNC(int, __fastcall, Lib_7A3090, 0x7A3090, (void* self, void* edx, int a1, int a2, int a3, int a4));

// Pure forwarding thunk: `push ebp; mov ebp, esp; pop ebp; jmp 0x7A3090`.
static int __fastcall ForwardToLib7A3090(void* self, void* edx, int a1, int a2, int a3, int a4)
{
    // the original tail-jumps with ecx/edx untouched: the callee takes `this` in ecx
    return Lib_7A3090(self, edx, a1, a2, a3, a4);
}

HOOK(0x750C60, ForwardToLib7A3090, "auto");
