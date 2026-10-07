#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall TCharTask_ForwardInnerVirtualCall(
    C_0082fbe8* self, void* edx)
{
    void* innerObject = CSF_AT(void*, self, 0x10);
    if (!innerObject)
        return 0;

    using VirtualTarget = int(__fastcall*)(C_0082fbe8*, void*);
    auto target = reinterpret_cast<VirtualTarget>(
        CSF_AT(void**, innerObject, 0)[8]);

    // Force a tail transfer, retaining the caller's stack and incoming EDX.
    [[clang::musttail]] return target(
        reinterpret_cast<C_0082fbe8*>(innerObject), edx);
}

HOOK(0x004DB260, TCharTask_ForwardInnerVirtualCall, "auto");
