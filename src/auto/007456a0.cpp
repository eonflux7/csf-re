#include <cstdint>
#include "globals.h"
#include "hook.h"

static CSF_FUNC(void, __fastcall, NetPlayerDDL_ProcessObject, 0x007A5E80,
               (void* self, void* edx));

static void __stdcall NetPlayerDDL_ProcessCallback(
    int /*a1*/, int object, int /*a3*/, int /*a4*/, int /*a5*/)
{
    NetPlayerDDL_ProcessObject(reinterpret_cast<void*>(object), nullptr);
}

HOOK(0x007456A0, NetPlayerDDL_ProcessCallback, "auto");
