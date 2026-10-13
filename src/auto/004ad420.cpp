#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Callee bind. The original forwards PrepareStaticAiState's eax to its caller
// through a tail jump, so the bind returns int to carry that value through.
CSF_FUNC(int, __stdcall, PrepareStaticAiState, 0x004DC3C0, (void* a1));

// Original: push ebp / mov ebp,esp / pop ebp / jmp 0x4DC3C0 (tail jump).
// The single stack argument is forwarded unchanged; the callee pops 4 and so
// does this function (ret 4 equivalent). The callee's result is returned as-is.
static int __stdcall PrepareStaticAiStateForward(void* a1) {
    return PrepareStaticAiState(a1);
}

HOOK(0x4AD420, PrepareStaticAiStateForward, "auto");
