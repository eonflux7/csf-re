#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(char, __fastcall, NetPlayerDdl_ProcessField, 0x00744650, (void* self, void* edx, int a2, void* a3));

static char __stdcall NetPlayerDdl_ForwardToProcessField(void* a1, int a2, void* a3)
{
    return NetPlayerDdl_ProcessField(a1, nullptr, a2, a3);
}

HOOK(0x00745990, NetPlayerDdl_ForwardToProcessField, "auto");
