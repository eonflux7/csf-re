#include "hook.h"
#include "globals.h"

static CSF_FUNC(void, __fastcall, NetPlayerDdl_ProcessObject, 0x00751A60,
               (void* self, void* edx));

static void __stdcall NetPlayerDdl_ProcessCallback(
    int /*a1*/, void* object, int /*a3*/, int /*a4*/, int /*a5*/)
{
    NetPlayerDdl_ProcessObject(object, nullptr);
}

HOOK(0x00745530, NetPlayerDdl_ProcessCallback, "auto");
