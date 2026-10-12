#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, sub_746890, 0x00746890, (void* self, void* edx, void* a2));

static void __stdcall NetPlayerDdl_ForwardToFirstArgument(
    C_008622e8* destination, C_008622e8* source)
{
    sub_746890(destination, nullptr, source);
}

HOOK(0x00747440, NetPlayerDdl_ForwardToFirstArgument, "auto");
