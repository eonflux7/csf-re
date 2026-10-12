#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, NetPlayerDdl_ProcessObject, 0x007455C0, (void* self, void* edx, void* a2));

static int __stdcall NetPlayerDdl_ForwardToSecondArg(
    int a1, C_008622e8* arg4, int a3, C_008622e8* a2, int a5)
{
    (void)a1;
    (void)a3;
    (void)a5;
    return NetPlayerDdl_ProcessObject(arg4, nullptr, a2);
}

HOOK(0x007455A0, NetPlayerDdl_ForwardToSecondArg, "auto");
