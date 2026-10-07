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

// Tail-jump target (lib): declared __stdcall, callee pops 8 bytes (2 stack args).
CSF_FUNC(int, __stdcall, ForwardTarget_7A3310, 0x7A3310, (int a1, int a2));

// 0x750CD0 is a pure thunk: push ebp; mov ebp, esp; pop ebp; jmp 0x7A3310.
// It forwards both stack arguments to 0x7A3310, which itself cleans the two
// arguments off the stack (ret 8). Reimplemented here as a __stdcall wrapper
// taking the same two arguments and returning the forwarded result.
static int __stdcall ForwardToHandler(int a1, int a2)
{
    return ForwardTarget_7A3310(a1, a2);
}

HOOK(0x750CD0, ForwardToHandler, "auto");
