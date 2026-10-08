#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Tail-call forwarder for 0x541230. The original is `push ebp; mov ebp,esp; pop ebp; jmp 0x541230`.
// The push/pop pair is a net no-op on ebp and the stack, so the thunk only needs to transfer
// control. ebx and the two stack arguments are preserved, and no stack frame is created.
// The target address is loaded into eax and reached through an indirect jmp, which keeps the
// caller's return address and stack arguments in place for the callee.
__declspec(naked) static void MenuMpStats_ForwardToLayout541230() {
    __asm {
        mov eax, 0x00541230
        jmp eax
    }
}

HOOK(0x00541570, MenuMpStats_ForwardToLayout541230, "auto");
