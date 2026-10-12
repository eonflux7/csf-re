#include "hook.h"
#include "globals.h"

CSF_FUNC(int, __fastcall, NetPlayerDDL_ProcessForwardedArguments, 0x0074B310, (void* self, void* edx, int a2, int a3, int a4, int a5));

static int __stdcall NetPlayerDDL_ForwardWithSelf(
    int /*unused1*/, void* self, int /*unused3*/,
    int arg4, int arg5, int arg6, int arg7)
{
    return NetPlayerDDL_ProcessForwardedArguments(
        self, nullptr, arg6, arg4, arg5, arg7);
}

HOOK(0x00745650, NetPlayerDDL_ForwardWithSelf, "auto");
