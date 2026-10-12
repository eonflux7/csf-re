#include "hook.h"
#include "globals.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, NetPlayerDDL_ForwardOperation, 0x00751910, (void* self, void* edx, void* a2, int a3, int a4, int a5));

static void* __stdcall NetPlayerDDL_ForwardWithSelf(
    int /*a1*/, C_008622e8* self, int /*unused10*/,
    int a3, int a4, C_008622e8* a2, int a5)
{
    return NetPlayerDDL_ForwardOperation(self, nullptr, a2, a3, a4, a5);
}

HOOK(0x00745580, NetPlayerDDL_ForwardWithSelf, "auto");
