#include "hook.h"

// Stdcall identity helper from the MSVC runtime: returns its single stack argument unchanged.
// The callee pops its 4-byte argument (ret 4). It never reads ecx, so it has no `this` parameter.
static int __stdcall PassThroughIntArg(int a1)
{
    return a1;
}

HOOK(0x4A31A0, PassThroughIntArg, "auto");
