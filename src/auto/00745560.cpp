#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __fastcall, sub_655E40, 0x00655E40, (void* self, void* edx, char* a2));

static int __stdcall NetPlayerDdl_ForwardToFourthArg(
    int /*a1*/, char* a2, int /*a3*/, void* a4, int /*a5*/)
{
    return sub_655E40(a4, nullptr, a2);
}

HOOK(0x00745560, NetPlayerDdl_ForwardToFourthArg, "auto");
