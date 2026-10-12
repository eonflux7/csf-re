#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(char, __fastcall, NetPlayerDdl_ProcessField, 0x00747210, (void* self, void* edx, int a5, int a3));

static char __stdcall NetPlayerDdl_ForwardToProcessField(void* object, int argument2, int argument3)
{
    return NetPlayerDdl_ProcessField(object, nullptr, argument2, argument3);
}

HOOK(0x00747460, NetPlayerDdl_ForwardToProcessField, "auto");
