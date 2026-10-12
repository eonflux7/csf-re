#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(void, __fastcall, ClearByteFlag20C, 0x00751820, (void* self, void*));

static void __stdcall NetPlayerDdl_ClearPlayerFlag(
    int /*a1*/, int player, int /*a3*/, int /*a4*/, int /*a5*/)
{
    ClearByteFlag20C(reinterpret_cast<void*>(player), nullptr);
}

HOOK(0x00745640, NetPlayerDdl_ClearPlayerFlag, "auto");
