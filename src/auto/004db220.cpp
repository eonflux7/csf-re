#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static int __fastcall C_0082fbe8_ForwardInnerVirtualQuery(
    C_0082fbe8* self, void* edx)
{
    auto* inner = CSF_AT(C_0082fbe8*, self, 0x10);
    if (!inner)
        return 0;

    // Require a tail jump; preserve EDX and the incoming stack layout.
    [[clang::musttail]]
    return reinterpret_cast<int (__fastcall*)(C_0082fbe8*, void*)>(
        CSF_AT(void**, inner, 0)[3])(inner, edx);
}

HOOK(0x004DB220, C_0082fbe8_ForwardInnerVirtualQuery, "auto");
