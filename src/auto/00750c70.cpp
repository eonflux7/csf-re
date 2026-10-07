#include <cstdint>
#include "globals.h"
#include "hook.h"

// The original thunk is a pure tail-call: it forwards its single stack argument
// straight to 0x007A3190, which performs the stack cleanup (ret 4) on its behalf.
CSF_FUNC(int, __stdcall, Sub_7A3190, 0x7A3190, (int a1));

static int __stdcall ForwardTo7A3190(int a1)
{
    return Sub_7A3190(a1);
}

HOOK(0x750C70, ForwardTo7A3190, "auto");
