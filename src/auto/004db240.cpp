#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static int __fastcall C_0082fbe8_InvokeInnerVirtual(C_0082fbe8* self, void* edx)
{
    auto* inner = CSF_AT(C_0082fbe8*, self, 0x10);
    if (!inner)
        return 0;

    using VirtualMethod = int(__fastcall*)(C_0082fbe8*, void*);
    [[clang::musttail]] return
        reinterpret_cast<VirtualMethod>(CSF_AT(void**, inner, 0)[5])(inner, edx);
}

HOOK(0x4DB240, C_0082fbe8_InvokeInnerVirtual, "auto");
