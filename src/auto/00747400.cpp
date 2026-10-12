#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, NetPlayerDdl_ProcessField, 0x00746C80, (void* self, void* edx, void* arg0, int a3));

static char __stdcall NetPlayerDdl_ForwardToProcessField(
    C_00858180* source, C_00858180* destination, int field)
{
    return NetPlayerDdl_ProcessField(source, nullptr, destination, field);
}

HOOK(0x00747400, NetPlayerDdl_ForwardToProcessField, "auto");
