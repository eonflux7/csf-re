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

// Thunk at 0x0074E6B0:
//   push ebp
//   mov  ebp, esp
//   pop  ebp
//   jmp  0x0065E700
// It performs no work of its own; it simply forwards its three integer
// arguments (popped by the callee, __stdcall) to the handler at 0x0065E700.
CSF_FUNC(int, __stdcall, Sub_65E700, 0x65E700, (int a1, int a2, int a3));

static int __stdcall ForwardCallTo65E700(int a1, int a2, int a3)
{
    return Sub_65E700(a1, a2, a3);
}

HOOK(0x74E6B0, ForwardCallTo65E700, "auto");
