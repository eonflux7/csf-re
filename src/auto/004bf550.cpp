#include "hook.h"
#include "globals.h"

// Forwards the first stack argument to the worker at 0x4BF4D0 and returns its result.
CSF_FUNC(int, __stdcall, sub_4BF4D0, 0x004BF4D0, (int a1));

static int __stdcall AiCoreDebug_ForwardFirstArg(int a1, int /*a2*/, int /*a3*/)
{
    return sub_4BF4D0(a1);
}

HOOK(0x004BF550, AiCoreDebug_ForwardFirstArg, "auto");
