#include "hook.h"
#include "globals.h"

// Forwards the first stack argument to the worker at 0x4C5B80 and returns its result.
CSF_FUNC(int, __stdcall, sub_4C5B80, 0x004C5B80, (int a1));

static int __stdcall AiCoverAttack_ForwardFirstArg(int a1, int /*a2*/, int /*a3*/)
{
    return sub_4C5B80(a1);
}

HOOK(0x004C5C50, AiCoverAttack_ForwardFirstArg, "auto");
