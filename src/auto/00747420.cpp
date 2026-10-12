#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, NetPlayerDdl_ProcessField, 0x00746490, (void* self, void* edx, void* a2, int a3));

static void __stdcall NetPlayerDdl_ForwardToProcessField(
    C_008622e8* destination, int fieldIndex, C_008622e8* source)
{
    NetPlayerDdl_ProcessField(destination, nullptr, source, fieldIndex);
}

HOOK(0x00747420, NetPlayerDdl_ForwardToProcessField, "auto");
