#include <cstdint>
#include "globals.h"
#include "hook.h"

// 0x67F530 is the tail-call target of the thunk at 0x750C80
// (IDA/Hex-Rays shows it as j_j_nullsub_19). It shares the thunk's
// signature: three stack arguments, callee-popped (__stdcall).
CSF_FUNC(int, __stdcall, Sub_67F530, 0x67F530, (int a1, int a2, int a3));

// The original at 0x750C80 only performs a frame setup/teardown and then
// jumps straight to 0x67F530, forwarding all three arguments unchanged.
static int __stdcall ForwardToSub_67F530(int a1, int a2, int a3)
{
    return Sub_67F530(a1, a2, a3);
}

HOOK(0x750C80, ForwardToSub_67F530, "auto");
