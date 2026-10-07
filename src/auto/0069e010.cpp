#include "hook.h"

// 0x69E010: trivial pass-through that returns the first stack argument and
// ignores the second one; the callee pops both arguments (ret 8).
static int __stdcall GetFirstArgument(int first, int /*second*/)
{
    return first;
}

HOOK(0x69E010, GetFirstArgument, "auto");
